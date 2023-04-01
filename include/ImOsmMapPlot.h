#pragma once
#include "ImOsmCoords.h"
#include "ImOsmITileLoader.h"
#include <implot.h>
#include <memory>

namespace ImOsm {
class MapPlot {
public:
  MapPlot();
  MapPlot(std::shared_ptr<ITileLoader> &loader);
  virtual ~MapPlot() = default;

  virtual void paint();
  virtual void paintOverMap() {}

  inline void resetBounds();

  // Geo CS routines
  inline void setBoundsGeo(float minLat,
                           float maxLat,
                           float minLon,
                           float maxLon);

  inline void getBoundsGeo(float &minLat,
                           float &maxLat,
                           float &minLon,
                           float &maxLon) const;

  inline float minLat() const { return _minLat; }
  inline float maxLat() const { return _maxLat; }
  inline float minLon() const { return _minLon; }
  inline float maxLon() const { return _maxLon; }

  inline float mouseLat() const { return _mouseLat; }
  inline float mouseLon() const { return _mouseLon; }

  inline bool inBoundsGeo(float lat, float lon) const {
    return lat > _minLat && lat < _maxLat && lon > _minLon && lon < _maxLon;
  }

  // Tile CS routines
  inline void getBoundsTile(int &minTX, int &maxTX, int &minTY,
                            int &maxTY) const;

  inline int minTileX() const { return _minTX; };
  inline int maxTileX() const { return _maxTX; };
  inline int minTileY() const { return _minTY; };
  inline int maxTileY() const { return _maxTY; };

  inline int zoom() const { return _zoom; }

  // Local CS routines
  inline void setBoundsLocal(float minX, float maxX, float minY, float maxY);

  inline void getBoundsLocal(float &minX, float &maxX, float &minY,
                             float &maxY) const;

  inline int minLocalX() const { return _minX; };
  inline int maxLocalX() const { return _maxX; };
  inline int minLocalY() const { return _minY; };
  inline int maxLocalY() const { return _maxY; };

  inline bool inBoundsLocal(float x, float y) const {
    return x > _minX && x < _maxX && y > _minY && y < _maxY;
  }

private:
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
  float _minLat{MinLat}, _maxLat{MaxLat};
  float _minLon{MinLon}, _maxLon{MaxLon};
  float _minX{}, _maxX{};
  float _minY{}, _maxY{};
  int _minTX{}, _maxTX{};
  int _minTY{}, _maxTY{};
  int _zoom{};
  float _pixelsX{}, _pixelsY{};
  float _rangeX{}, _rangeY{};
  float _resX{}, _resY{};
  int _tilesNum{};
  float _mouseLat{}, _mouseLon{};

  enum class SetBounds { None, Geo, Local };
  SetBounds _setBounds{SetBounds::None};

  std::shared_ptr<ITileLoader> _loader;
};

inline void MapPlot::resetBounds() {
  _minLat = MinLat;
  _maxLat = MaxLat;
  _minLon = MinLon;
  _maxLon = MaxLon;
  _setBounds = SetBounds::Geo;
}

inline void MapPlot::setBoundsGeo(float minLat,
                                  float maxLat,
                                  float minLon,
                                  float maxLon) {
  _minLat = minLat;
  _maxLat = maxLat;
  _minLon = minLon;
  _maxLon = maxLon;
  _setBounds = SetBounds::Geo;
}

inline void MapPlot::getBoundsGeo(float &minLat,
                                  float &maxLat,
                                  float &minLon,
                                  float &maxLon) const {
  minLat = _minLat;
  maxLat = _maxLat;
  minLon = _minLon;
  maxLon = _maxLon;
}

inline void MapPlot::getBoundsTile(int &minTX, int &maxTX, int &minTY,
                                   int &maxTY) const {
  minTX = _minTX;
  maxTX = _maxTX;
  minTY = _minTY;
  maxTY = _maxTY;
}

inline void MapPlot::setBoundsLocal(float minX, float maxX, float minY,
                                    float maxY) {
  _minX = minX;
  _maxX = maxX;
  _minY = minY;
  _maxY = maxY;
  _setBounds = SetBounds::Local;
}

inline void MapPlot::getBoundsLocal(float &minX, float &maxX, float &minY,
                                    float &maxY) const {
  minX = _minX;
  maxX = _maxX;
  minY = _minY;
  maxY = _maxY;
}
} // namespace ImOsm
