#pragma once
#include <memory>
#include <string>

namespace ImOsm {
class MapPlot;
class ITileLoader;

class TileSourceWidget {
public:
  TileSourceWidget(std::shared_ptr<MapPlot> mapPlot);
  ~TileSourceWidget();

  void paint();

private:
  std::shared_ptr<MapPlot> _mapPlot;
  std::shared_ptr<ITileLoader> _tileLoader;
  std::string _source;
  int _requestLimit{10};
};
} // namespace ImOsm
