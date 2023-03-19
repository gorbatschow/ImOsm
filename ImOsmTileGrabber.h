#pragma once

#include "ImOsmITileSource.h"
#include <future>
#include <memory>
#include <thread>

namespace ImOsm {
class TileGrabber {
public:
  TileGrabber();
  TileGrabber(std::shared_ptr<ITileSource> source);

  void grab(int minZ, int maxZ, float minLon, float maxLon, float minLat,
            float maxLat);

private:
  std::shared_ptr<ITileSource> _source;
  std::string _tileSavePath{std::filesystem::current_path().string() +
                            "/tiles"};
  struct FutureData {};
  std::future<FutureData> _future;

  FutureData onLaunchGrab(int minZ, int maxZ, float minLon, float maxLon,
                          float minLat, float maxLat);
  void waitAndSave() const;
};

} // namespace ImOsm
