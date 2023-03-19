#pragma once
#include "ImOsmCoords.h"
#include "ImOsmTileSource.h"
#include "ImOsmTileSourceUrl.h"
#include <memory>
#include <thread>

namespace ImOsm {
class TileGrabber {
public:
  TileGrabber() : _source{std::make_shared<TileSourceUrl>()} {}

  TileGrabber(std::shared_ptr<ITileSource> source) : _source{source} {}

  void grab(int minZ, int maxZ, float minLon, float maxLon, float minLat,
            float maxLat) {
    _future = std::async(std::launch::async, &TileGrabber::onLaunchGrab, this,
                         minZ, maxZ, minLon, maxLon, minLat, maxLat);
  }

private:
  std::shared_ptr<ITileSource> _source;

  std::string _tileSavePath{std::filesystem::current_path().string() +
                            "/tiles"};

  struct FutureData {};
  std::future<FutureData> _future;

  FutureData onLaunchGrab(int minZ, int maxZ, float minLon, float maxLon,
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

  void waitAndSave() const {
    std::vector<std::shared_ptr<Tile>> tiles;
    while (!_source->takeAll(tiles)) {
      std::this_thread::sleep_for(1s);
    }
    for (const auto &tile : tiles) {
      tile->saveFile(_tileSavePath, _source->tileExtension());
    }
  }
};

} // namespace ImOsm
