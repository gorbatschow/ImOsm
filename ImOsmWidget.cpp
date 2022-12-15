#include "ImOsmWidget.h"
#include "OsmCoords.h"
#include <algorithm>
#include <implot.h>

ImOsmWidget::ImOsmWidget() {}

void ImOsmWidget::paint() {
  ImGui::Begin("OsmPlotWidget");

  ImGui::Text("TILE PX: %.2f", _plotTileSizePx);

  if (ImPlot::BeginPlot("##OsmPlot", {-1, -1},
                        ImPlotFlags_Equal | ImPlotFlags_NoLegend)) {
    ImPlot::SetupAxis(ImAxis_Y1, NULL, ImPlotAxisFlags_Invert);

    const auto plotLimits = ImPlot::GetPlotLimits(ImAxis_X1, ImAxis_Y1);
    const auto plotSizePx = ImPlot::GetPlotSize();
    auto Z = float(1 << _zoom);
    _plotTileSizePx = plotSizePx.x / plotLimits.Size().x / Z;
    if (_plotTileSizePx > _tileSizePx * _zoomCoef) {
      Z = float(1 << ++_zoom);
    } else if (_plotTileSizePx < _tileSizePx) {
      Z = float(1 << --_zoom);
    }
    _zoom = std::clamp(_zoom, 0, LimZoom);

    _minLon =
        std::clamp(x2lon(float(plotLimits.X.Min) * Z, _zoom), -LimLon, LimLon);
    _maxLon =
        std::clamp(x2lon(float(plotLimits.X.Max) * Z, _zoom), -LimLon, LimLon);
    _minLat =
        std::clamp(y2lat(float(plotLimits.Y.Min) * Z, _zoom), -LimLat, LimLat);
    _maxLat =
        std::clamp(y2lat(float(plotLimits.Y.Max) * Z, _zoom), -LimLat, LimLat);

    _minTX = lon2tx(_minLon, _zoom);
    _maxTX = lon2tx(_maxLon, _zoom);
    _minTY = lat2ty(_minLat, _zoom);
    _maxTY = lat2ty(_maxLat, _zoom);

    _loader.beginLoad(_zoom, _minTX, _maxTX, _minTY, _maxTY);

    const ImVec2 uv0(0, 1), uv1(1, 0);
    const ImVec4 tint(1, 1, 1, 1);
    ImVec2 bmin, bmax;
    for (auto x = _minTX; x != _maxTX + 1; ++x) {
      bmin.x = float(x) / Z;
      bmax.x = float(x + 1) / Z;
      for (auto y = _minTY; y != _maxTY + 1; ++y) {
        bmin.y = float(y) / Z;
        bmax.y = float(y + 1) / Z;
        ImPlot::PlotImage("##", _loader.tileAt(_zoom, x, y), bmin, bmax, uv0,
                          uv1, tint);
      }
    }

    ImPlot::EndPlot();
  }

  ImGui::End();
}
