#pragma once
#include "ImOsmTileSource.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <future>
using namespace std::chrono_literals;

namespace ImOsm {
class TileSourceAsync : public ITileSource {
public:
  TileSourceAsync() = default;
  virtual ~TileSourceAsync() = default;

  void setRequestLimit() {}

  virtual bool hasRequest() override { return !_requests.empty(); }

  virtual bool canRequest() override {
    return _requests.size() < _requestLimit;
  }

  virtual bool canTakeAll() override {
    return std::all_of(
        std::begin(_requests), std::end(_requests), [](AsyncTile &tile) {
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

  virtual bool takeAll(std::vector<std::shared_ptr<Tile>> &tiles) override {
    if (canTakeAll()) {
      for (auto &request : _requests) {
        tiles.push_back(request.makeTile());
      }
      _requests.clear();
      return true;
    }
    return false;
  }

protected:
  struct AsyncTile {
    struct FutureData {
      std::vector<std::byte> blob;
      bool ok{};
    };

    int z{}, x{}, y{};
    std::future<FutureData> future;

    bool operator==(const AsyncTile &other) const {
      return this->z == other.z && this->x == other.x && this->y == other.y;
    }
    bool operator==(const std::array<int, 3> &zxy) const {
      return this->z == zxy[0] && this->x == zxy[1] && this->y == zxy[2];
    }

    std::shared_ptr<Tile> makeTile() {
      assert(future.wait_for(0s) == std::future_status::ready);
      const auto data{future.get()};
      return std::make_shared<Tile>(z, x, y, data.blob, data.ok);
    }
  };
  std::vector<AsyncTile> _requests;
  int _requestLimit{10};

  virtual AsyncTile::FutureData onHandleRequest(int z, int x, int y) = 0;
};
} // namespace ImOsm
