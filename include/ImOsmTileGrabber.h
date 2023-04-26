#pragma once
#include <atomic>
#include <filesystem>
#include <future>
#include <memory>

namespace ImOsm {
class ITileSource;
class ITileSaver;

class TileGrabber {
public:
  TileGrabber(std::shared_ptr<ITileSource> source,
              std::shared_ptr<ITileSaver> saver);
  ~TileGrabber();

  void grab(const double minLat, const double maxLat, const double minLon,
            const double maxLon, const int minZ, const int maxZ);

  void stop();

  inline int tileCounter() const { return _tileCounter.load(); }

private:
  struct FutureData {};
  FutureData onLaunchGrab(const double minLat, const double maxLat,
                          const double minLon, const double maxLon,
                          const int minZ, const int maxZ);

private:
  std::shared_ptr<ITileSource> _source;
  std::shared_ptr<ITileSaver> _saver;
  std::future<FutureData> _future;
  std::atomic_int _tileCounter{0};
  std::atomic_bool _stop{false};
};

} // namespace ImOsm
