#pragma once
#include "ImOsmTile.h"
#include <memory>
#include <vector>

namespace ImOsm {
class ITileSource {
public:
  virtual ~ITileSource() = default;

  virtual bool hasRequest() = 0;
  virtual bool canRequest() = 0;
  virtual bool request(int z, int x, int y) = 0;
  virtual bool canTakeAll() = 0;
  virtual bool takeAll(std::vector<std::shared_ptr<Tile>> &tiles) = 0;

  virtual const std::string &tileExtension() const = 0;
};

} // namespace ImOsm
