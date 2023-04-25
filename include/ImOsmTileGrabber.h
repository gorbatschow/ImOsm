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

  TileGrabber(std::shared_ptr<ITileSource> source);

  TileGrabber(std::shared_ptr<ITileSource> source,
              std::shared_ptr<ITileSaver> saver);

  void grab(const double minLat, const double maxLat, const double minLon,
            const double maxLon, const int minZ, const int maxZ);

  void stop();

  inline int tileCounter() const { return _tileCounter.load(); }

private:
  std::shared_ptr<ITileSource> _source;
  std::shared_ptr<ITileSaver> _saver;

  struct FutureData {};
  std::future<FutureData> _future;

  FutureData onLaunchGrab(const double minLat, const double maxLat,
                          const double minLon, const double maxLon,
                          const int minZ, const int maxZ);

  std::atomic_int _tileCounter{0};
  std::atomic_bool _stop{false};
};

} // namespace ImOsm
