#pragma once
#include "ImOsmTileSourceUrl.h"
#include <array>

namespace ImOsm {
class TileSourceUrlCustom : public TileSourceUrl {
public:
  TileSourceUrlCustom(const std::string &prefix, const std::string &suffix,
                      const std::array<uint8_t, 3> &order = {0, 1, 2})
      : _prefix{prefix}, _suffix{suffix}, _order{order} {}

protected:
  virtual std::string makeUrl(int z, int x, int y) override {
    const std::array<int, 3> zxy{z, x, y};
    std::ostringstream urlmaker;
    urlmaker << _prefix;
    urlmaker << '/' << zxy[_order[0]] << '/' << zxy[_order[1]] << '/'
             << zxy[_order[2]] << _suffix;
    return urlmaker.str();
  }

private:
  std::string _prefix;
  std::string _suffix;
  std::array<uint8_t, 3> _order;
};

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
