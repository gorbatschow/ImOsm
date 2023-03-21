#pragma once
#include <memory>

namespace ImOsm {
class ITile;
class ITileSaver {
public:
  virtual ~ITileSaver() = default;
  virtual bool save(std::shared_ptr<ITile> tile) = 0;
};
} // namespace ImOsm
