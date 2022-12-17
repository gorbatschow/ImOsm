#pragma once
#include "OsmTileLoader.h"
#include <implot.h>

class ImOsmWidget {
public:
  ImOsmWidget();

  void paint();

  inline void setBounds();
  inline void setBounds(double minLon, double maxLon, double minLat,
                        double maxLat);
  inline void getBounds(double &minLon, double &maxLon, double &minLat,
                        double &maxLat) const;
  inline void getBounds(int &minTX, int &maxTX, int &minTY, int &maxTY) const;
  inline double minLon() const { return _minLon; }
  inline double maxLon() const { return _maxLon; }
  inline double minLat() const { return _minLat; }
  inline double maxLat() const { return _maxLat; }
  inline int zoom() const { return int(_zoom); }
  inline double mouseLon() const { return _mouseLon; }
  inline double mouseLat() const { return _mouseLat; }

private:
  constexpr static const float LimLat{85.0};
  constexpr static const float LimLon{179.9};
  constexpr static const int LimZoom{18};

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

  OsmTileLoader _loader;
};

inline void ImOsmWidget::setBounds() {
  _minLon = -LimLon;
  _maxLon = LimLon;
  _minLat = -LimLat;
  _maxLat = LimLat;
  _setBounds = true;
}

inline void ImOsmWidget::setBounds(double minLon, double maxLon, double minLat,
                                   double maxLat) {
  _minLon = minLon;
  _maxLon = maxLon;
  _minLat = minLat;
  _maxLat = maxLat;
  _setBounds = true;
}

inline void ImOsmWidget::getBounds(double &minLon, double &maxLon,
                                   double &minLat, double &maxLat) const {
  minLon = _minLon;
  maxLon = _maxLon;
  minLat = _minLat;
  maxLat = _maxLat;
}

inline void ImOsmWidget::getBounds(int &minTX, int &maxTX, int &minTY,
                                   int &maxTY) const {
  minTX = _minTX;
  maxTX = _maxTX;
  minTY = _minTY;
  maxTY = _maxTY;
}
