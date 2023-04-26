#include "ImOsmTileSourceUrlImpl.h"
#include <sstream>

namespace ImOsm {
TileSourceUrlCustom::TileSourceUrlCustom(int request_limit, bool preload,
                                         const std::string &url_tpl)
    : TileSourceUrl{request_limit, preload}, _url_tpl{url_tpl} {}

std::string TileSourceUrlCustom::makeUrl(int z, int x, int y) {
  std::string url{_url_tpl};
  const auto pos_z{url.find("$Z$")};
  if (pos_z < url.size()) {
    url.replace(pos_z, 3, std::to_string(z));
    const auto pos_x{url.find("$X$")};
    if (pos_x < url.size()) {
      url.replace(pos_x, 3, std::to_string(x));
      const auto pos_y{url.find("$Y$")};
      if (pos_y < url.size()) {
        url.replace(pos_y, 3, std::to_string(y));
      }
    }
  }
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
