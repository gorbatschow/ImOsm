#include "ImOsmTileGrabber.h"
#include "ImOsmCoords.h"
#include "ImOsmTileSourceUrl.h"

namespace ImOsm {
TileGrabber::TileGrabber() : _source{std::make_shared<TileSourceUrl>()} {}

TileGrabber::TileGrabber(std::shared_ptr<ITileSource> source)
    : _source{source} {}

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
        if (_source->canRequest()) {
          _source->request(z, x, y);
        } else {
          waitAndSave();
        }
      }
    }
  }
  waitAndSave();
  return data;
}

void TileGrabber::waitAndSave() const {
  std::vector<std::shared_ptr<Tile>> tiles;
  while (!_source->takeAll(tiles)) {
    std::this_thread::sleep_for(1s);
  }
  for (const auto &tile : tiles) {
    tile->saveFile(_tileSavePath, _source->tileExtension());
  }
}

} // namespace ImOsm
