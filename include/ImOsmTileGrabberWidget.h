#pragma once
#include <ini.h>
#include <memory>

namespace ImOsm {
class MapPlot;
class TileGrabber;
class MapPlot;

class TileGrabberWidget {
public:
  TileGrabberWidget(std::shared_ptr<MapPlot> mapPlot);
  ~TileGrabberWidget();

  void loadState(const mINI::INIStructure &ini);
  void saveState(mINI::INIStructure &ini) const;
  void paint();

private:
  std::unique_ptr<TileGrabber> _tileGrabber;
  std::shared_ptr<MapPlot> _mapPlot;

  struct Ui;
  std::unique_ptr<Ui> _ui;
};
} // namespace ImOsm
