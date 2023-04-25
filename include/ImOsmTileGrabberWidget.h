#pragma once
#include "ImOsmMapPlot.h"
#include "ImOsmTileGrabber.h"
#include "ImOsmTileSaver.h"
#include "ImOsmTileSourceUrlImpl.h"
#include <cmath>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <string>

namespace ImOsm {
class TileGrabberWidget {
public:
  TileGrabberWidget(std::shared_ptr<MapPlot> mapPlot) : _mapPlot{mapPlot} {}

  void paint() {
    ImGui::PushID(this);
    ImGui::TextUnformatted("Tile Grabber");
    ImGui::Text("Min Lat %.6f, Min Lon %.6f", _mapPlot->minLat(),
                _mapPlot->minLon());
    ImGui::Text("Max Lat %.6f, Max Lon %.6f", _mapPlot->maxLat(),
                _mapPlot->maxLon());
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Min Z %d", _mapPlot->zoom());
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("Max Z", &_maxZ);
    _maxZ = std::clamp(_maxZ, _mapPlot->zoom(), MaxZoom);
    ImGui::SameLine();
    ImGui::Text("Tiles Count: %d",
                countTiles(_mapPlot->minLat(), _mapPlot->maxLat(),
                           _mapPlot->minLon(), _mapPlot->maxLon(),
                           _mapPlot->zoom(), _maxZ));

    ImGui::InputText("Tile Server URL##", &_url);

    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("Max. Requests", &_requestLimit);
    _requestLimit = std::clamp(_requestLimit, 1, 99);
    ImGui::SameLine();
    if (ImGui::Button("OSM")) {
      _url = TileSourceUrlOsm::URL_TPL;
    };
    ImGui::SameLine();
    if (ImGui::Button("ARC")) {
      _url = TileSourceUrlArcImagery::URL_TPL;
    };

    ImGui::InputText("Destination Dir##", &_dirname);
    if (ImGui::Button("Grab")) {
      _tileTotal =
          countTiles(_mapPlot->minLat(), _mapPlot->maxLat(), _mapPlot->minLon(),
                     _mapPlot->maxLon(), _mapPlot->zoom(), _maxZ);
      _tileGrabber = std::make_unique<TileGrabber>(
          std::make_shared<TileSourceUrlCustom>(_requestLimit, _preload, _url),
          std::make_shared<TileSaverSubDir>(_dirname));
      _tileGrabber->grab(_mapPlot->minLat(), _mapPlot->maxLat(),
                         _mapPlot->minLon(), _mapPlot->maxLon(),
                         _mapPlot->zoom(), _maxZ);
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop") && _tileGrabber) {
      _tileGrabber->stop();
    }

    if (_tileGrabber) {
      _tileCount = _tileGrabber->tileCounter();
    }
    _progress = float(_tileCount) / float(_tileTotal);
    _progress = std::isnan(_progress) ? 0.f : _progress;
    ImGui::SameLine();
    ImGui::Text("%d/%d", _tileCount, _tileTotal);
    ImGui::SameLine();
    ImGui::ProgressBar(_progress);

    ImGui::PopID();
  }

private:
  std::string _url{TileSourceUrlOsm::URL_TPL};
  std::string _dirname{"tiles"};
  std::unique_ptr<TileGrabber> _tileGrabber;
  std::shared_ptr<MapPlot> _mapPlot;
  int _requestLimit{10};
  const bool _preload{false};
  int _maxZ{18};
  int _tileCount{0}, _tileTotal{};
  float _progress{};
};
} // namespace ImOsm
