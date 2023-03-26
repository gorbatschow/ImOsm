#pragma once
#include <array>
#include <future>
#include <memory>
#include <vector>

namespace ImOsm {
class ITile;
using namespace std::chrono_literals;

struct TileAsync {
  struct FutureData {
    std::shared_ptr<ITile> tile;
  };

  int z{}, x{}, y{};
  std::future<FutureData> future;

  bool ready() const {
    return future.wait_for(0s) == std::future_status::ready;
  }

  bool operator==(const TileAsync &other) const {
    return this->z == other.z && this->x == other.x && this->y == other.y;
  }

  bool operator==(const std::array<int, 3> &zxy) const {
    return this->z == zxy[0] && this->x == zxy[1] && this->y == zxy[2];
  }
};
} // namespace ImOsm
