#pragma once
#include "ImOsmMapPlot.h"
#include "ImOsmTileGrabber.h"
#include "ImOsmTileSaver.h"
#include "ImOsmTileSourceUrlImpl.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <string>

namespace ImOsm {
class TileGrabberWidget
{
public:
  TileGrabberWidget(std::shared_ptr<MapPlot> mapPlot)
      : _mapPlot{mapPlot} {}

  void paint() {
    ImGui::TextUnformatted("Tile Grabber");
    ImGui::Text("Min Lat %.6f, Min Lon %.6f",
                _mapPlot->minLat(),
                _mapPlot->minLon());
    ImGui::Text("Max Lat %.6f, Max Lon %.6f",
                _mapPlot->maxLat(),
                _mapPlot->maxLon());
    ImGui::Text("Max Z %d", _mapPlot->zoom());

    ImGui::InputText("Tile Server URL##", &_url);
    if (ImGui::Button("OSM##")) {
      _url = "https://a.tile.openstreetmap.org/$Z$/$X$/&Y&.png";
    };
    ImGui::SameLine();
    if (ImGui::Button("ARC##")) {
      _url = "https://server.arcgisonline.com/ArcGIS/rest/services/"
             "World_Imagery/MapServer/tile/$Z$/$Y$/$X$";
    };

    ImGui::InputText("Destination Dir##", &_dirname);
    if (ImGui::Button("Grab")) {
      _tileGrabber = std::make_unique<TileGrabber>(
          std::make_shared<TileSourceUrlCustom>(_url),
          std::make_shared<TileSaverSubDir>(_dirname));
      _tileGrabber->grab(_mapPlot->zoom(),
                         ImOsm::MaxZoom,
                         _mapPlot->minLon(),
                         _mapPlot->maxLon(),
                         _mapPlot->minLat(),
                         _mapPlot->maxLat());
    }
  }

private:
  std::string _url;
  std::string _dirname;
  std::unique_ptr<TileGrabber> _tileGrabber;
  std::shared_ptr<MapPlot> _mapPlot;
};
} // namespace ImOsm
