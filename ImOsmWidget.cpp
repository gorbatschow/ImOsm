#include "ImOsmWidget.h"
#include "OsmCoords.h"
#include <algorithm>
#include <implot.h>

ImOsmWidget::ImOsmWidget() {}

void ImOsmWidget::paint() {
  if (ImPlot::BeginPlot("##OsmPlot", {-1, -1},
                        ImPlotFlags_Equal | ImPlotFlags_NoLegend)) {

    ImPlot::SetupAxis(ImAxis_X1, NULL, ImPlotAxisFlags_NoInitialFit);
    ImPlot::SetupAxis(ImAxis_Y1, NULL,
                      ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoInitialFit);
    ImPlot::SetupFinish();

    const auto plotLims = ImPlot::GetPlotLimits(ImAxis_X1, ImAxis_Y1);
    const auto plotSize = ImPlot::GetPlotSize();

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
    _zoom = std::clamp(int(floor(log2(_resX / _tilePixels))), 0, 18);
    _tilesNum = (1 << _zoom);
    _tileSize = 1.0 / float(_tilesNum);

    _minLon = std::clamp(x2lon(_minX * _tilesNum, _zoom), -LimLon, LimLon);
    _maxLon = std::clamp(x2lon(_maxX * _tilesNum, _zoom), -LimLon, LimLon);
    _minLat = std::clamp(y2lat(_minY * _tilesNum, _zoom), -LimLat, LimLat);
    _maxLat = std::clamp(y2lat(_maxY * _tilesNum, _zoom), -LimLat, LimLat);

    _minTX = lon2tx(std::clamp(_minLon, -LimLon, LimLon), _zoom);
    _maxTX = lon2tx(std::clamp(_maxLon, -LimLon, LimLon), _zoom);
    _minTY = lat2ty(std::clamp(_minLat, -LimLat, LimLat), _zoom);
    _maxTY = lat2ty(std::clamp(_maxLat, -LimLat, LimLat), _zoom);

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
