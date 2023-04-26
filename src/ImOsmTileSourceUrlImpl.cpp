#include "ImOsmTileSourceUrlImpl.h"
#include <sstream>

namespace ImOsm {
TileSourceUrlCustom::TileSourceUrlCustom(int request_limit, bool preload,
                                         const std::string &url_tpl)
    : TileSourceUrl{request_limit, preload}, _url_tpl{url_tpl} {}

std::string TileSourceUrlCustom::makeUrl(int z, int x, int y) {
  std::string url{_url_tpl};
  url.replace(url.find("$Z$"), 3, std::to_string(z));
  url.replace(url.find("$X$"), 3, std::to_string(x));
  url.replace(url.find("$Y$"), 3, std::to_string(y));
  return url;
}

// -----------------------------------------------------------------------------

TileSourceUrlOsm::TileSourceUrlOsm(int request_limit, bool preload)
    : TileSourceUrl{request_limit, preload} {}

std::string TileSourceUrlOsm::makeUrl(int z, int x, int y) {
  std::ostringstream urlmaker;
  urlmaker << "https://a.tile.openstreetmap.org";
  urlmaker << '/' << z << '/' << x << '/' << y << ".png";
  return urlmaker.str();
}
// -----------------------------------------------------------------------------

TileSourceUrlArcImagery::TileSourceUrlArcImagery(int request_limit,
                                                 bool preload)
    : TileSourceUrl{request_limit, preload} {}

std::string TileSourceUrlArcImagery::makeUrl(int z, int x, int y) {
  std::ostringstream urlmaker;
  urlmaker << "https://server.arcgisonline.com/ArcGIS/rest/"
              "services/World_Imagery/MapServer/tile";
  urlmaker << '/' << z << '/' << y << '/' << x;
  return urlmaker.str();
}

} // namespace ImOsm
