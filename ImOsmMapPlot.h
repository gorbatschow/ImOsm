#pragma once
#include "ImOsmTileLoader.h"
#include <implot.h>

namespace ImOsm {
class MapPlot {
public:
  MapPlot();
  virtual ~MapPlot() = default;

  virtual void paint();
  virtual void paintBeforeMap() {}
  virtual void paintOverMap() {}

  inline void setBounds();

  inline void setBoundsGeo(float minLon, float maxLon, float minLat,
                           float maxLat);

  inline void getBoundsGeo(float &minLon, float &maxLon, float &minLat,
                           float &maxLat) const;

  inline void getBoundsTile(int &minTX, int &maxTX, int &minTY,
                            int &maxTY) const;

  inline void setBoundsLocal(float &minX, float &maxX, float &minY,
                             float &maxY) const;

  inline void getBoundsLocal(float &minX, float &maxX, float &minY,
                             float &maxY) const;

  inline float minLon() const { return _minLon; }
  inline float maxLon() const { return _maxLon; }
  inline float minLat() const { return _minLat; }
  inline float maxLat() const { return _maxLat; }
  inline int zoom() const { return int(_zoom); }
  inline float mouseLon() const { return _mouseLon; }
  inline float mouseLat() const { return _mouseLat; }

  inline const TileLoader &loader() const { return _loader; };

private:
  constexpr static const float LimLat{85.0};
  constexpr static const float LimLon{179.9};
  constexpr static const int LimZoom{18};

  constexpr static const ImPlotFlags _plotFlags{ImPlotFlags_Equal |
                                                ImPlotFlags_NoLegend};

  constexpr static const ImPlotAxisFlags _xFlags{
      ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoGridLines |
      ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels |
      ImPlotAxisFlags_NoInitialFit | ImPlotAxisFlags_NoMenus |
      ImPlotAxisFlags_NoMenus | ImPlotAxisFlags_NoHighlight};

  constexpr static const ImPlotAxisFlags _yFlags{_xFlags |
                                                 ImPlotAxisFlags_Invert};

  constexpr static const ImVec2 _uv0{0, 1}, _uv1{1, 0};
  constexpr static const ImVec4 _tint{1, 1, 1, 1};

  ImPlotPoint _mousePos{};
  ImPlotRect _plotLims{};
  ImVec2 _plotSize{};

  float _tilePixels{256.0};
  float _tileSize{};
  float _minLon{-LimLon}, _maxLon{LimLon};
  float _minLat{-LimLat}, _maxLat{LimLat};
  float _minX{}, _maxX{};
  float _minY{}, _maxY{};
  int _minTX{}, _maxTX{};
  int _minTY{}, _maxTY{};
  float _pixelsX{}, _pixelsY{};
  float _rangeX{}, _rangeY{};
  float _resX{}, _resY{};
  int _tilesNum{};
  int _zoom{0};
  bool _setBounds{true};
  float _mouseLon{}, _mouseLat{};
  TileLoader _loader;
};

inline void MapPlot::setBounds() {
  _minLon = -LimLon;
  _maxLon = LimLon;
  _minLat = -LimLat;
  _maxLat = LimLat;
  _setBounds = true;
}

inline void MapPlot::setBoundsGeo(float minLon, float maxLon, float minLat,
                                  float maxLat) {
  _minLon = minLon;
  _maxLon = maxLon;
  _minLat = minLat;
  _maxLat = maxLat;
  _setBounds = true;
}

inline void MapPlot::getBoundsGeo(float &minLon, float &maxLon, float &minLat,
                                  float &maxLat) const {
  minLon = _minLon;
  maxLon = _maxLon;
  minLat = _minLat;
  maxLat = _maxLat;
}

inline void MapPlot::getBoundsTile(int &minTX, int &maxTX, int &minTY,
                                   int &maxTY) const {
  minTX = _minTX;
  maxTX = _maxTX;
  minTY = _minTY;
  maxTY = _maxTY;
}

inline void MapPlot::getBoundsLocal(float &minX, float &maxX, float &minY,
                                    float &maxY) const {
  minX = _minX;
  maxX = _maxX;
  minY = _minY;
  maxY = _maxY;
}
} // namespace ImOsm
