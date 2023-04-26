#pragma once

namespace ImOsm {
using ImTextureID = void *;
using size_t = unsigned long;

class ITile {
public:
  virtual ~ITile() = default;

  virtual int z() const = 0;
  virtual int x() const = 0;
  virtual int y() const = 0;

  virtual bool isTileZXY(int z, int x, int y) const = 0;
  virtual bool inBounds(int z, int xmin, int xmax, int ymin,
                        int ymax) const = 0;

  virtual bool isDummy() const = 0;

  virtual const char *rawBlob() const = 0;
  virtual size_t rawBlobSize() const = 0;

  virtual void rgbaLoad() const = 0;
  virtual const char *rgbaBlob() const = 0;
  virtual size_t rgbaBlobSize() const = 0;

  virtual ImTextureID texture() const = 0;
};
} // namespace ImOsm
