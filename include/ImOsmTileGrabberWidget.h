#pragma once
#include <memory>

namespace ImOsm {
class MapPlot;
class TileGrabber;
class MapPlot;

class TileGrabberWidget {
public:
  TileGrabberWidget(std::shared_ptr<MapPlot> mapPlot);
  ~TileGrabberWidget();

  void paint();

private:
  std::unique_ptr<TileGrabber> _tileGrabber;
  std::shared_ptr<MapPlot> _mapPlot;

  struct Ui;
  std::unique_ptr<Ui> _ui;
};
} // namespace ImOsm
