#include "ImOsmMapPlot.h"
#include "ImOsmCoords.h"
#include "ImOsmTileLoaderImpl.h"
#include <algorithm>
#include <implot.h>

namespace ImOsm {

struct MapPlot::Impl {
  constexpr static const ImPlotFlags plotFlags{ImPlotFlags_Equal |
                                               ImPlotFlags_NoLegend};

  constexpr static const ImPlotAxisFlags xFlags{
      ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoGridLines |
      ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels |
      ImPlotAxisFlags_NoInitialFit | ImPlotAxisFlags_NoMenus |
      ImPlotAxisFlags_NoMenus | ImPlotAxisFlags_NoHighlight};

  constexpr static const ImPlotAxisFlags yFlags{xFlags |
                                                ImPlotAxisFlags_Invert};

  constexpr static const ImVec2 uv0{0, 1}, uv1{1, 0};
  constexpr static const ImVec4 tint{1, 1, 1, 1};

  ImPlotPoint mousePos{};
  ImPlotRect plotLims{};
  ImVec2 plotSize{};
};

MapPlot::MapPlot()
    : _impl{std::make_unique<Impl>()},
      _loader{std::make_shared<TileLoaderOsmMap>()} {}

MapPlot::MapPlot(std::shared_ptr<ITileLoader> &loader)
    : _impl{std::make_unique<Impl>()}, _loader{loader}, _minLat{MIN_LAT},
      _minLon{MIN_LON}, _maxLat{MAX_LAT}, _maxLon{MAX_LON} {
  resetBounds();
}

MapPlot::~MapPlot() = default;

void MapPlot::paint() {
  if (ImPlot::BeginPlot("##ImOsmMapPlot", {-1, -1}, _impl->plotFlags)) {

    ImPlot::SetupAxis(ImAxis_X1, nullptr, _impl->xFlags);
    ImPlot::SetupAxis(ImAxis_Y1, nullptr, _impl->yFlags);
    ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0.0, 1.0);

    if (_setBounds != SetBounds::None) {
      if (_setBounds == SetBounds::Geo) {
        _minX = lon2x(_minLon, 0);
        _maxX = lon2x(_maxLon, 0);
        _minY = lat2y(_minLat, 0);
        _maxY = lat2y(_maxLat, 0);
      } else if (_setBounds == SetBounds::Local) {
        // do nothing
      }
      ImPlot::SetupAxisLimits(ImAxis_X1, _minX, _maxX, ImPlotCond_Always);
      ImPlot::SetupAxisLimits(ImAxis_Y1, _minY, _maxY, ImPlotCond_Always);
      _setBounds = SetBounds::None;
    }

    ImPlot::SetupFinish();

    _impl->mousePos = ImPlot::GetPlotMousePos(ImAxis_X1, ImAxis_Y1);
    _impl->plotLims = ImPlot::GetPlotLimits(ImAxis_X1, ImAxis_Y1);
    _impl->plotSize = ImPlot::GetPlotSize();

    _mouseLon = x2lon(_impl->mousePos.x, 0);
    _mouseLat = y2lat(_impl->mousePos.y, 0);

    _pixelsX = _impl->plotSize.x;
    _pixelsY = _impl->plotSize.y;

    _minX = _impl->plotLims.X.Min;
    _maxX = _impl->plotLims.X.Max;
    _minY = _impl->plotLims.Y.Min;
    _maxY = _impl->plotLims.Y.Max;
    _rangeX = fabs(_maxX - _minX);
    _rangeY = fabs(_maxY - _minY);

    _resX = _pixelsX / _rangeX;
    _resY = _pixelsY / _rangeY;
    _zoom =
        std::clamp(int(floor(log2(_resX / _tilePixels))), MIN_ZOOM, MAX_ZOOM);
    _tilesNum = POW2[_zoom];
    _tileSize = 1.0 / float(_tilesNum);

    const auto minMaxLat{std::minmax(y2lat(_minY * _tilesNum, _zoom),
                                     y2lat(_maxY * _tilesNum, _zoom))};
    const auto minMaxLon{std::minmax(x2lon(_minX * _tilesNum, _zoom),
                                     x2lon(_maxX * _tilesNum, _zoom))};

    _minLat = minMaxLat.first;
    _maxLat = minMaxLat.second;
    _minLon = minMaxLon.first;
    _maxLon = minMaxLon.second;

    _minTX = std::clamp(int(_minX * _tilesNum), 0, _tilesNum - 1);
    _maxTX = std::clamp(int(_maxX * _tilesNum), 0, _tilesNum - 1);
    _minTY = std::clamp(int(_minY * _tilesNum), 0, _tilesNum - 1);
    _maxTY = std::clamp(int(_maxY * _tilesNum), 0, _tilesNum - 1);

    _loader->beginLoad(_zoom, _minTX, _maxTX, _minTY, _maxTY);

    ImVec2 bmin{float(_minTX), float(_minTY)};
    ImVec2 bmax{float(_maxTX), float(_maxTY)};

    for (auto x{_minTX}; x != _maxTX + 1; ++x) {
      bmin.x = float(x) * _tileSize;
      bmax.x = float(x + 1) * _tileSize;
      for (auto y{_minTY}; y != _maxTY + 1; ++y) {
        bmin.y = float(y) * _tileSize;
        bmax.y = float(y + 1) * _tileSize;
        ImPlot::PlotImage("##", _loader->tileAt(_zoom, x, y), bmin, bmax,
                          _impl->uv0, _impl->uv1, _impl->tint);
      }
    }

    _loader->endLoad();

    paintOverMap();

    ImPlot::EndPlot();
  }
}

void ImOsm::MapPlot::resetBounds() {
  _minLat = MIN_LAT;
  _maxLat = MAX_LAT;
  _minLon = MIN_LON;
  _maxLon = MAX_LON;
  _setBounds = SetBounds::Geo;
}

void ImOsm::MapPlot::setBoundsGeo(float minLat, float maxLat, float minLon,
                                  float maxLon) {
  _minLat = minLat;
  _maxLat = maxLat;
  _minLon = minLon;
  _maxLon = maxLon;
  _setBounds = SetBounds::Geo;
}

void ImOsm::MapPlot::getBoundsGeo(float &minLat, float &maxLat, float &minLon,
                                  float &maxLon) const {
  minLat = _minLat;
  maxLat = _maxLat;
  minLon = _minLon;
  maxLon = _maxLon;
}

void ImOsm::MapPlot::getBoundsTile(int &minTX, int &maxTX, int &minTY,
                                   int &maxTY) const {
  minTX = _minTX;
  maxTX = _maxTX;
  minTY = _minTY;
  maxTY = _maxTY;
}

void ImOsm::MapPlot::setBoundsLocal(float minX, float maxX, float minY,
                                    float maxY) {
  _minX = minX;
  _maxX = maxX;
  _minY = minY;
  _maxY = maxY;
  _setBounds = SetBounds::Local;
}

void ImOsm::MapPlot::getBoundsLocal(float &minX, float &maxX, float &minY,
                                    float &maxY) const {
  minX = _minX;
  maxX = _maxX;
  minY = _minY;
  maxY = _maxY;
}

bool MapPlot::mouseOnPlot() const {
  return _impl->mousePos.x > _impl->plotLims.X.Min &&
         _impl->mousePos.x < _impl->plotLims.X.Max &&
         _impl->mousePos.y > _impl->plotLims.Y.Min &&
         _impl->mousePos.y < _impl->plotLims.Y.Max;
}

} // namespace ImOsm
