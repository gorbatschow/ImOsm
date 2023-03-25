#pragma once
#include "ImOsmITileSaver.h"
#include "ImOsmITileSource.h"
#include "ImOsmTile.h"
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

  virtual bool hasRequest() override { return !_requests.empty(); }

  virtual bool hasRequest(int z, int x, int y) override {
    const auto it{std::find(_requests.begin(), _requests.end(),
                            std::array<int, 3>{z, x, y})};
    return it != _requests.end();
  }

  virtual bool canRequest() override {
    return _requests.size() < _requestLimit;
  }

  virtual bool request(int z, int x, int y) override {
    if (canRequest()) {
      _requests.push_back(TileAsync{
          z, x, y,
          std::async(std::launch::async, &TileSourceAsync::onHandleRequest,
                     this, z, x, y)});
      return true;
    }
    return false;
  }

  virtual bool canTakeAll() override {
    return std::all_of(std::begin(_requests), std::end(_requests),
                       [](TileAsync &tile) { return tile.ready(); });
  }

  virtual bool takeAll(std::vector<std::shared_ptr<ITile>> &tiles) override {
    if (canTakeAll()) {
      for (auto &request : _requests) {
        tiles.push_back(request.future.get().tile);
      }
      _requests.clear();
      return true;
    }
    return false;
  }

  virtual bool takeReady(std::vector<std::shared_ptr<ITile>> &tiles) override {
    const bool sz = _requests.size();
    for (auto it{_requests.begin()}; it != _requests.end();) {
      if (it->ready()) {
        tiles.push_back((*it).future.get().tile);
        it = _requests.erase(it);
      } else {
        ++it;
      }
    }
    return sz != _requests.size() && sz != 0;
  }

  virtual bool saveAll(std::shared_ptr<ITileSaver> saver) override {
    std::vector<std::shared_ptr<ITile>> tiles;
    while (!takeAll(tiles)) {
      std::this_thread::sleep_for(1s);
    }
    return saver->saveMulti(tiles);
  }

protected:
  virtual bool receiveTile(int z, int x, int y,
                           std::vector<std::byte> &blob) = 0;

private:
  TileAsync::FutureData onHandleRequest(int z, int x, int y) {
    std::vector<std::byte> blob;
    TileAsync::FutureData data;
    if (receiveTile(z, x, y, blob)) {
      data.tile = std::make_shared<Tile>(z, x, y, blob);
    } else {
      data.tile = std::make_shared<TileDummy>(z, x, y);
    }
    return data;
  }

  std::vector<TileAsync> _requests;
  int _requestLimit{20};
};
} // namespace ImOsm
