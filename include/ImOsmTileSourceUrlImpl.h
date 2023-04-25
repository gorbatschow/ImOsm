#pragma once
#include "ImOsmTileSourceUrl.h"
#include <array>

// https://www.arcgis.com/apps/mapviewer/index.html
// See Basemap

namespace ImOsm {
class TileSourceUrlCustom : public TileSourceUrl {
public:
  TileSourceUrlCustom(int request_limit, bool preload,
                      const std::string &url_tpl)
      : TileSourceUrl{request_limit, preload}, _url_tpl{url_tpl} {}

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
public:
  TileSourceUrlOsm(int request_limit, bool preload)
      : TileSourceUrl{request_limit, preload} {}

protected:
  virtual std::string makeUrl(int z, int x, int y) override {
    std::ostringstream urlmaker;
    urlmaker << "https://a.tile.openstreetmap.org";
    urlmaker << '/' << z << '/' << x << '/' << y << ".png";
    return urlmaker.str();
  }

public:
  inline static const char URL_TPL[]{
      "https://a.tile.openstreetmap.org/$Z$/$X$/$Y$.png"};
};

// -----------------------------------------------------------------------------

class TileSourceUrlArcImagery : public TileSourceUrl {
public:
  TileSourceUrlArcImagery(int request_limit, bool preload)
      : TileSourceUrl{request_limit, preload} {}

protected:
  virtual std::string makeUrl(int z, int x, int y) override {
    std::ostringstream urlmaker;
    urlmaker << "https://server.arcgisonline.com/ArcGIS/rest/"
                "services/World_Imagery/MapServer/tile";
    urlmaker << '/' << z << '/' << y << '/' << x;
    return urlmaker.str();
  }

public:
  inline static const char URL_TPL[]{
      "https://server.arcgisonline.com/ArcGIS/rest/services/"
      "World_Imagery/MapServer/tile/$Z$/$Y$/$X$"};
};
using TileSourceUrlArc = TileSourceUrlArcImagery;

} // namespace ImOsm
