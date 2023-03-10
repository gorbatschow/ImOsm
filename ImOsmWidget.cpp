#include "ImOsmWidget.h"
#include "OsmCoords.h"
#include <algorithm>
#include <implot.h>

ImOsmWidget::ImOsmWidget() {}

void ImOsmWidget::paint() {
  if (ImPlot::BeginPlot("##OsmPlot", {-1, -1},
                        ImPlotFlags_Equal | ImPlotFlags_NoLegend)) {

    const auto flags = ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoGridLines |
                       ImPlotAxisFlags_NoTickMarks |
                       ImPlotAxisFlags_NoTickLabels |
                       ImPlotAxisFlags_NoInitialFit | ImPlotAxisFlags_NoMenus |
                       ImPlotAxisFlags_NoMenus | ImPlotAxisFlags_NoHighlight;
    ImPlot::SetupAxis(ImAxis_X1, NULL, flags);
    ImPlot::SetupAxis(ImAxis_Y1, NULL, flags | ImPlotAxisFlags_Invert);
    ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0.0, 1.0);

    if (_setBounds) {
      _minX = lon2x(_minLon, 0);
      _maxX = lon2x(_maxLon, 0);
      _minY = lat2y(_minLat, 0);
      _maxY = lat2y(_maxLat, 0);
      ImPlot::SetupAxisLimits(ImAxis_X1, _minX, _maxX, ImPlotCond_Always);
      ImPlot::SetupAxisLimits(ImAxis_Y1, _minY, _maxY, ImPlotCond_Always);
      _setBounds = false;
    }

    ImPlot::SetupFinish();

    const auto mousePos = ImPlot::GetPlotMousePos(ImAxis_X1, ImAxis_Y1);
    const auto plotLims = ImPlot::GetPlotLimits(ImAxis_X1, ImAxis_Y1);
    const auto plotSize = ImPlot::GetPlotSize();

    _mouseLon = x2lon(mousePos.x, 0);
    _mouseLat = y2lat(mousePos.y, 0);

    _pixelsX = plotSize.x;
    _pixelsY = plotSize.y;

    _minX = plotLims.X.Min;
    _maxX = plotLims.X.Max;
    _minY = plotLims.Y.Min;
    _maxY = plotLims.Y.Max;
    _rangeX = abs(_maxX - _minX);
    _rangeY = abs(_maxY - _minY);

    _resX = _pixelsX / _rangeX;
    _resY = _pixelsY / _rangeY;
    _zoom = std::clamp(int(floor(log2(_resX / _tilePixels))), 0, LimZoom);
    _tilesNum = (1 << _zoom);
    _tileSize = 1.0 / float(_tilesNum);

    _minLon = x2lon(_minX * _tilesNum, _zoom);
    _maxLon = x2lon(_maxX * _tilesNum, _zoom);
    _minLat = y2lat(_minY * _tilesNum, _zoom);
    _maxLat = y2lat(_maxY * _tilesNum, _zoom);

    _minTX = std::clamp(lon2tx(_minLon, _zoom), 0, _tilesNum - 1);
    _maxTX = std::clamp(lon2tx(_maxLon, _zoom), 0, _tilesNum - 1);
    _minTY = std::clamp(lat2ty(_minLat, _zoom), 0, _tilesNum - 1);
    _maxTY = std::clamp(lat2ty(_maxLat, _zoom), 0, _tilesNum - 1);

    _loader.beginLoad(_zoom, _minTX, _maxTX, _minTY, _maxTY);

    const ImVec2 uv0(0, 1), uv1(1, 0);
    const ImVec4 tint(1, 1, 1, 1);
    ImVec2 bmin{float(_minTX), float(_minTY)};
    ImVec2 bmax{float(_maxTX), float(_maxTY)};

    for (auto x = _minTX; x != _maxTX + 1; ++x) {
      bmin.x = float(x) * _tileSize;
      bmax.x = float(x + 1) * _tileSize;
      for (auto y = _minTY; y != _maxTY + 1; ++y) {
        bmin.y = float(y) * _tileSize;
        bmax.y = float(y + 1) * _tileSize;
        ImPlot::PlotImage("##", _loader.tileAt(_zoom, x, y), bmin, bmax, uv0,
                          uv1, tint);
      }
    }

    ImPlot::EndPlot();
  }
}
