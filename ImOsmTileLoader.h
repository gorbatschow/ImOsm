#pragma once
#include "ImOsmITileLoader.h"
#include "ImOsmTileSourceUrl.h"

namespace ImOsm {
class TileLoader : public ITileLoader {
public:
  TileLoader() {}

  virtual void beginLoad(int z, int xmin, int xmax, int ymin, int ymax) {}
  virtual ImTextureID tileAt(int z, int x, int y) = 0;
  virtual int tileCount() const { return 0; };
  virtual void endLoad() {}

private:
};
}; // namespace ImOsm
