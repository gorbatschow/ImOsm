#pragma once
#include <memory>

namespace ImOsm {
class ITileLoader;

class MapPlot {
public:
  MapPlot();
  MapPlot(std::shared_ptr<ITileLoader> &loader);
  virtual ~MapPlot();

  inline void setTileLoader(std::shared_ptr<ITileLoader> loader) {
    _loader = loader;
  }

  virtual void paint();
  virtual void paintOverMap() {}

  inline void resetBounds();

  // Geo CS routines
  void setBoundsGeo(float minLat, float maxLat, float minLon, float maxLon);
  void getBoundsGeo(float &minLat, float &maxLat, float &minLon,
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
  void getBoundsTile(int &minTX, int &maxTX, int &minTY, int &maxTY) const;

  inline int minTileX() const { return _minTX; };
  inline int maxTileX() const { return _maxTX; };
  inline int minTileY() const { return _minTY; };
  inline int maxTileY() const { return _maxTY; };

  inline int zoom() const { return _zoom; }

  // Local CS routines
  void setBoundsLocal(float minX, float maxX, float minY, float maxY);
  void getBoundsLocal(float &minX, float &maxX, float &minY, float &maxY) const;

  inline int minLocalX() const { return _minX; };
  inline int maxLocalX() const { return _maxX; };
  inline int minLocalY() const { return _minY; };
  inline int maxLocalY() const { return _maxY; };

  inline bool inBoundsLocal(float x, float y) const {
    return x > _minX && x < _maxX && y > _minY && y < _maxY;
  }

  bool mouseOnPlot() const;

private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
  std::shared_ptr<ITileLoader> _loader, _prevLoader;

  float _tilePixels{256.0};
  float _tileSize{};
  float _minLat{}, _maxLat{};
  float _minLon{}, _maxLon{};
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
  float _mouseClickedLat{}, _mouseClickedLon{};

  enum class SetBounds { None, Geo, Local };
  SetBounds _setBounds{SetBounds::None};
};

} // namespace ImOsm
