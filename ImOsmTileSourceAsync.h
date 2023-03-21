#pragma once
#include "ImOsmITileSaver.h"
#include "ImOsmITileSource.h"
#include "ImOsmTileAsync.h"
#include <algorithm>
#include <array>
#include <future>
#include <thread>

namespace ImOsm {
using namespace std::chrono_literals;

class TileSourceAsync : public ITileSource {
public:
  TileSourceAsync() = default;
  virtual ~TileSourceAsync() = default;

  void setRequestLimit(int limit) { _requestLimit = limit; }
  int requestLimit() const { return _requestLimit; }

  virtual bool hasRequest() override { return !_requests.empty(); }

  virtual bool canRequest() override {
    return _requests.size() < _requestLimit;
  }

  virtual bool canTakeAll() override {
    return std::all_of(
        std::begin(_requests), std::end(_requests), [](TileAsync &tile) {
          return tile.future.wait_for(0s) == std::future_status::ready;
        });
  }

  virtual bool request(int z, int x, int y) override {
    if (canRequest()) {
      if (_requests.size() < _requestLimit) {
        _requests.push_back(
            {z, x, y,
             std::async(std::launch::async, &TileSourceAsync::onHandleRequest,
                        this, z, x, y)});
      }
      return true;
    }
    return false;
  }

  virtual bool takeAll(std::vector<std::shared_ptr<ITile>> &tiles) override {
    if (canTakeAll()) {
      for (auto &request : _requests) {
        tiles.push_back(request.try_get());
      }
      _requests.clear();
      return true;
    }
    return false;
  }

  virtual bool takeAll(std::shared_ptr<ITileSaver> saver) override {
    std::vector<std::shared_ptr<ITile>> tiles;
    while (!takeAll(tiles)) {
      std::this_thread::sleep_for(1s);
    }
    for (auto tile : tiles) {
      if (!saver->save(tile)) {
        return false;
      }
    }
    return true;
  }

protected:
  std::vector<TileAsync> _requests;
  int _requestLimit{10};

  virtual TileAsync::FutureData onHandleRequest(int z, int x, int y) = 0;
};
} // namespace ImOsm
