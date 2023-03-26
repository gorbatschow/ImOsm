#pragma once
#include "ImOsmTileSourceUrl.h"
#include <array>

// https://www.arcgis.com/apps/mapviewer/index.html
// See Basemap

namespace ImOsm {
class TileSourceUrlCustom : public TileSourceUrl {
public:
  TileSourceUrlCustom(const std::string &url_tpl) : _url_tpl{url_tpl} {}

protected:
  virtual std::string makeUrl(int z, int x, int y) override {
    std::string url{_url_tpl};
    url.replace(url.find("$Z$"), 3, std::to_string(z));
    url.replace(url.find("$X$"), 3, std::to_string(x));
    url.replace(url.find("$Y$"), 3, std::to_string(y));
    return url;
  }

private:
  const std::string _url_tpl;
};

// -----------------------------------------------------------------------------

class TileSourceUrlOsm : public TileSourceUrl {
protected:
  virtual std::string makeUrl(int z, int x, int y) override {
    std::ostringstream urlmaker;
    urlmaker << "https://a.tile.openstreetmap.org";
    urlmaker << '/' << z << '/' << x << '/' << y << ".png";
    return urlmaker.str();
  }
};

// -----------------------------------------------------------------------------

class TileSourceUrlArcImagery : public TileSourceUrl {
protected:
  virtual std::string makeUrl(int z, int x, int y) override {
    std::ostringstream urlmaker;
    urlmaker << "https://server.arcgisonline.com/ArcGIS/rest/"
                "services/World_Imagery/MapServer/tile";
    urlmaker << '/' << z << '/' << y << '/' << x;
    return urlmaker.str();
  }
};

} // namespace ImOsm
