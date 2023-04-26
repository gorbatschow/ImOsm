#pragma once
#include "ImOsmMapPlot.h"
#include "ImOsmTileLoaderImpl.h"
#include <memory>
#include <misc/cpp/imgui_stdlib.h>

namespace ImOsm {
class TileSourceWidget {
public:
  TileSourceWidget(std::shared_ptr<MapPlot> mapPlot) : _mapPlot{mapPlot} {
    _mapPlot->setTileLoader(std::make_shared<TileLoaderUrlMap>(_source));
  }

  void paint() {
    ImGui::PushID(this);
    ImGui::TextUnformatted("Tile Source");
    if (ImGui::Button("Apply")) {
      if (_source.starts_with("http")) {
        _tileLoader =
            std::make_shared<TileLoaderUrlMap>(_source, _requestLimit);
      } else if (!_source.empty()) {
        _tileLoader = std::make_shared<TileLoaderFsMap>(_source, _requestLimit);
      }
      _mapPlot->setTileLoader(_tileLoader);
    }
    ImGui::SameLine();
    ImGui::InputText("Tile Source##", &_source);

    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("Max. Requests", &_requestLimit);
    _requestLimit = std::clamp(_requestLimit, 1, 99);
    ImGui::SameLine();
    if (ImGui::Button("OSM")) {
      _source = TileSourceUrlOsm::URL_TPL;
      _mapPlot->setTileLoader(std::make_shared<TileLoaderOsmMap>());
    };
    ImGui::SameLine();
    if (ImGui::Button("ARC")) {
      _source = TileSourceUrlArc::URL_TPL;
      _mapPlot->setTileLoader(std::make_shared<TileLoaderArcMap>());
    };

    ImGui::PopID();
  }

private:
  std::shared_ptr<MapPlot> _mapPlot;
  std::string _source{TileSourceUrlOsm::URL_TPL};
  std::shared_ptr<ITileLoader> _tileLoader;
  int _requestLimit{10};
};
} // namespace ImOsm
