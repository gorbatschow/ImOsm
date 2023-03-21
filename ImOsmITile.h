#pragma once
#include <vector>

namespace ImOsm {
enum class TileFormat { PNG };

class ITile {
public:
  virtual ~ITile() = default;

  virtual int z() const = 0;
  virtual int x() const = 0;
  virtual int y() const = 0;
  virtual const std::vector<std::byte> &blob() const = 0;

  virtual void loadTexture() = 0;
  virtual TileFormat format() const { return TileFormat::PNG; }
};
} // namespace ImOsm
