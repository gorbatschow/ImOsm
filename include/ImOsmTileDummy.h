#pragma once
#include "ImOsmITile.h"

namespace ImOsm {
class TileDummy : public ITile {
public:
  TileDummy(int z, int x, int y) : _z{z}, _x{x}, _y{y} {}
  virtual ~TileDummy() override = default;

  virtual int z() const override { return _z; }
  virtual int x() const override { return _x; }
  virtual int y() const override { return _y; }

  virtual bool isTileZXY(int z, int x, int y) const override {
    return _z == z && _x == x && y == _y;
  }

  virtual bool inBounds(int z, int xmin, int xmax, int ymin,
                        int ymax) const override {
    const bool cz{_z == z};
    const bool cx{_x >= xmin || _x <= xmax};
    const bool cy{_y >= ymin || _y <= ymax};
    return cz && cx && cy;
  }

  virtual bool isDummy() const override final { return rawBlob() == nullptr; }

  virtual const char *rawBlob() const override { return nullptr; }
  virtual size_t rawBlobSize() const override { return 0; }

  virtual void rgbaLoad() const override {}
  virtual const char *rgbaBlob() const override { return nullptr; }
  virtual size_t rgbaBlobSize() const override { return 0; }

  virtual ImTextureID texture() const override { return 0; }

private:
  int _z{}, _x{}, _y{};
};
} // namespace ImOsm
