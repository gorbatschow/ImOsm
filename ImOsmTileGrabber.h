#pragma once
#include "ImOsmITileSource.h"
#include <filesystem>
#include <future>
#include <memory>
#include <thread>

namespace ImOsm {
class TileGrabber {
public:
  TileGrabber();
  TileGrabber(std::shared_ptr<ITileSource> source,
              std::shared_ptr<ITileSaver> saver);

  void grab(int minZ, int maxZ, float minLon, float maxLon, float minLat,
            float maxLat);

private:
  std::shared_ptr<ITileSource> _source;
  std::shared_ptr<ITileSaver> _saver;

  struct FutureData {};
  std::future<FutureData> _future;

  FutureData onLaunchGrab(int minZ, int maxZ, float minLon, float maxLon,
                          float minLat, float maxLat);
};

} // namespace ImOsm
