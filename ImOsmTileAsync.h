#pragma once
#include "ImOsmTile.h"
#include <array>
#include <cassert>
#include <future>
#include <memory>
#include <vector>

namespace ImOsm {
using namespace std::chrono_literals;

class TileAsync {
public:
  struct FutureData {
    std::vector<std::byte> blob;
    bool ok{};
    TileFormat format{};
  };

  int z{}, x{}, y{};
  std::future<FutureData> future;

  bool operator==(const TileAsync &other) const {
    return this->z == other.z && this->x == other.x && this->y == other.y;
  }

  bool operator==(const std::array<int, 3> &zxy) const {
    return this->z == zxy[0] && this->x == zxy[1] && this->y == zxy[2];
  }

  std::shared_ptr<ITile> try_get() {
    if (future.wait_for(0s) == std::future_status::ready) {
      const auto data{future.get()};
      return std::make_shared<Tile>(z, x, y, data.blob, data.format, data.ok);
    }
    return nullptr;
  }
};
} // namespace ImOsm
