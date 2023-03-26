#include "ImOsmTileGrabber.h"
#include "ImOsmCoords.h"
#include "ImOsmTileSaver.h"
#include "ImOsmTileSourceUrlImpl.h"

namespace ImOsm {
TileGrabber::TileGrabber()
    : _source{std::make_shared<TileSourceUrlOsm>()},
      _saver{std::make_shared<TileSaverDir>()} {}

TileGrabber::TileGrabber(std::shared_ptr<ITileSource> source)
    : _source{source}, _saver{std::make_shared<TileSaverDir>()} {}

TileGrabber::TileGrabber(std::shared_ptr<ITileSource> source,
                         std::shared_ptr<ITileSaver> saver)
    : _source{source}, _saver{saver} {}

void TileGrabber::grab(int minZ, int maxZ, float minLon, float maxLon,
                       float minLat, float maxLat) {
  _future = std::async(std::launch::async, &TileGrabber::onLaunchGrab, this,
                       minZ, maxZ, minLon, maxLon, minLat, maxLat);
}

TileGrabber::FutureData TileGrabber::onLaunchGrab(int minZ, int maxZ,
                                                  float minLon, float maxLon,
                                                  float minLat, float maxLat) {
  FutureData data;
  for (auto z{minZ}; z != maxZ + 1; ++z) {
    const auto tilesNum{(1 << z)};
    const std::pair<int, int> tx{
        std::minmax(std::clamp(lon2tx(minLon, z), 0, tilesNum - 1),
                    std::clamp(lon2tx(maxLon, z), 0, tilesNum - 1))};
    const std::pair<int, int> ty{
        std::minmax(std::clamp(lat2ty(minLat, z), 0, tilesNum - 1),
                    std::clamp(lat2ty(maxLat, z), 0, tilesNum - 1))};
    for (auto x{tx.first}; x != tx.second + 1; ++x) {
      for (auto y{ty.first}; y != ty.second + 1; ++y) {
        if (!_source->canRequest()) {
          _source->saveAll(_saver);
        }
        _source->request(z, x, y);
      }
    }
  }
  _source->saveAll(_saver);
  return data;
}

} // namespace ImOsm
