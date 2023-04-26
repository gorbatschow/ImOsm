#include "ImOsmTileGrabber.h"
#include "ImOsmCoords.h"
#include "ImOsmITile.h"
#include "ImOsmITileSaver.h"
#include "ImOsmITileSource.h"

namespace ImOsm {
TileGrabber::TileGrabber(std::shared_ptr<ITileSource> source,
                         std::shared_ptr<ITileSaver> saver)
    : _source{source}, _saver{saver} {}

TileGrabber::~TileGrabber() {}

void TileGrabber::grab(const double minLat, const double maxLat,
                       const double minLon, const double maxLon, const int minZ,
                       const int maxZ) {
  _stop = false;
  _future = std::async(std::launch::async, &TileGrabber::onLaunchGrab, this,
                       minLat, maxLat, minLon, maxLon, minZ, maxZ);
}

void TileGrabber::stop() { _stop = true; }

TileGrabber::FutureData
TileGrabber::onLaunchGrab(const double minLat, const double maxLat,
                          const double minLon, const double maxLon,
                          const int minZ, const int maxZ) {
  _tileCounter = 0;
  std::vector<std::shared_ptr<ITile>> tiles;
  tiles.reserve(_source->requestLimit());
  FutureData data;

  for (auto z{minZ}; z != maxZ + 1; ++z) {
    const auto tx{minmax_tx(minLon, maxLon, z)};
    const auto ty{minmax_ty(minLat, maxLat, z)};
    for (auto x{tx.first}; x != tx.second + 1; ++x) {
      for (auto y{ty.first}; y != ty.second + 1; ++y) {
        if (_stop) {
          return data;
        }
        if (!_source->canRequest()) {
          tiles.clear();
          _source->takeAll(tiles);
          std::for_each(std::begin(tiles), std::end(tiles),
                        [this](const auto &tile) {
                          if (tile->isDummy()) {
                            _source->request(tile->z(), tile->x(), tile->y());
                          }
                        });
          std::erase_if(tiles,
                        [](const auto &tile) { return tile->isDummy(); });
          _source->takeAll(tiles);
          _saver->saveMulti(tiles);
          _tileCounter += tiles.size();
        }
        _source->request(z, x, y);
      }
    }
  }

  tiles.clear();
  _source->takeAll(tiles);
  _saver->saveMulti(tiles);
  _tileCounter += tiles.size();

  return data;
}

} // namespace ImOsm
