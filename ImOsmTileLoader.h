#pragma once
#include "ImOsmITileLoader.h"
#include "ImOsmITileSource.h"
#include "ImOsmTileSourceUrlImpl.h"

namespace ImOsm {
class TileLoader : public ITileLoader {
public:
  TileLoader() : _source{std::make_shared<TileSourceUrlArcImagery>()} {}

  TileLoader(std::shared_ptr<ITileSource> source) : _source{source} {}

  virtual void beginLoad(int z, int xmin, int xmax, int ymin,
                         int ymax) override {
    const auto cond{
        [z, xmin, xmax, ymin, ymax](const std::shared_ptr<ITile> &tile) {
          return !tile->inBounds(z, xmin, xmax, ymin, ymax) || tile->isDummy();
        }};
    _tiles.erase(std::remove_if(_tiles.begin(), _tiles.end(), cond),
                 _tiles.end());
    _source->takeReady(_tiles);
  }

  virtual ImTextureID tileAt(int z, int x, int y) override {
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

  virtual int tileCount() const override { return _tiles.size(); };

  virtual void endLoad() override {}

private:
  std::shared_ptr<ITileSource> _source;
  std::vector<std::shared_ptr<ITile>> _tiles;
};
}; // namespace ImOsm
