#include "ImOsmTileLoader.h"
#include "ImOsmITile.h"
#include "ImOsmITileSource.h"
#include <algorithm>

namespace ImOsm {
TileLoader::TileLoader(std::shared_ptr<ITileSource> source) : _source{source} {}

void ImOsm::TileLoader::beginLoad(int z, int xmin, int xmax, int ymin,
                                  int ymax) {
  const auto cond{
      [z, xmin, xmax, ymin, ymax](const std::shared_ptr<ITile> &tile) {
        return !tile->inBounds(z, xmin, xmax, ymin, ymax) || tile->isDummy();
      }};
  _tiles.erase(std::remove_if(_tiles.begin(), _tiles.end(), cond),
               _tiles.end());
  _source->takeReady(_tiles);
}

ImTextureID TileLoader::tileAt(int z, int x, int y) {
  const auto cond{[z, x, y](const std::shared_ptr<ITile> &tile) {
    return tile->isTileZXY(z, x, y);
  }};
  const auto it{std::find_if(_tiles.begin(), _tiles.end(), cond)};

  if (it != _tiles.end()) {
    if (!(*it)->isDummy()) {
      return (*it)->texture();
    }
  }

  if (!_source->hasRequest(z, x, y)) {
    _source->request(z, x, y);
  }

  return 0;
}
} // namespace ImOsm
