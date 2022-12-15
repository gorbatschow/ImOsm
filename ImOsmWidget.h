#pragma once
#include "OsmTileLoader.h"
#include <implot.h>

class ImOsmWidget {
public:
  ImOsmWidget();
  void paint();

private:
  constexpr static const float LimLat{85.0};
  constexpr static const float LimLon{179.9};
  constexpr static const int LimZoom{18};

  float _tileSizePx{256.0};
  float _plotTileSizePx{};
  float _zoomCoef{2.0};
  float _minLon{-LimLon}, _maxLon{LimLon};
  float _minLat{-LimLat}, _maxLat{LimLat};
  int _minTX{}, _maxTX{};
  int _minTY{}, _maxTY{};
  int _zoom{0};

  OsmTileLoader _loader;
};
