#pragma once
#include "ImOsmTileSourceUrl.h"

namespace ImOsm {
class TileSourceUrlOsm : public TileSourceUrl {
public:
  virtual std::string makeUrl(int z, int x, int y) override {
    std::ostringstream urlmaker;
    urlmaker << "https://a.tile.openstreetmap.org";
    urlmaker << '/' << z << '/' << x << '/' << y << ".png";
    return urlmaker.str();
  }
};

// https://www.arcgis.com/apps/mapviewer/index.html
// See Basemap

class TileSourceUrlArcImagery : public TileSourceUrl {
public:
  virtual std::string makeUrl(int z, int x, int y) override {
    std::ostringstream urlmaker;
    urlmaker << "https://server.arcgisonline.com/ArcGIS/rest/"
                "services/World_Imagery/MapServer/tile";
    urlmaker << '/' << z << '/' << y << '/' << x;
    return urlmaker.str();
  }
};

} // namespace ImOsm
