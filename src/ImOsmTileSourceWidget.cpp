#include "ImOsmTileSourceWidget.h"
#include "ImOsmMapPlot.h"
#include "ImOsmTileLoaderImpl.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace ImOsm {

struct TileSourceWidget::Ui {
  std::string source{TileSourceUrlOsm::URL_TPL};
  int requestLimit{10};
};

TileSourceWidget::TileSourceWidget(std::shared_ptr<MapPlot> mapPlot)
    : _mapPlot{mapPlot}, _ui{std::make_unique<Ui>()} {
  updateTileLoader();
}

void TileSourceWidget::loadState(const mINI::INIStructure &ini) {
  if (ini.has("tile_source")) {
    const auto opts{ini.get("tile_source")};
    if (opts.has("path_url")) {
      _ui->source = opts.get("path_url");
    }
    if (opts.has("request_limit")) {
      _ui->requestLimit = stoi(opts.get("request_limit"));
      std::clamp(_ui->requestLimit, 1, 99);
    }
  }
  updateTileLoader();
}

void TileSourceWidget::saveState(mINI::INIStructure &ini) const {
  ini["tile_source"].set("path_url", _ui->source);
  ini["tile_source"].set("request_limit", std::to_string(_ui->requestLimit));
}

TileSourceWidget::~TileSourceWidget() = default;

void TileSourceWidget::paint() {
  ImGui::PushID(this);

  ImGui::TextUnformatted("Tile Source");
  if (ImGui::Button("Apply")) {
    updateTileLoader();
  }
  ImGui::SameLine();
  ImGui::InputText("Path / URL", &_ui->source);

  ImGui::SetNextItemWidth(100);
  ImGui::InputInt("Max. Requests", &_ui->requestLimit);
  _ui->requestLimit = std::clamp(_ui->requestLimit, 1, 99);
  ImGui::SameLine();
  if (ImGui::Button("OSM")) {
    _ui->source = TileSourceUrlOsm::URL_TPL;
    _mapPlot->setTileLoader(
        std::make_shared<TileLoaderOsmMap>(_ui->requestLimit));
  };
  ImGui::SameLine();
  if (ImGui::Button("ARC")) {
    _ui->source = TileSourceUrlArc::URL_TPL;
    _mapPlot->setTileLoader(
        std::make_shared<TileLoaderArcMap>(_ui->requestLimit));
  };

  ImGui::PopID();
}

void TileSourceWidget::updateTileLoader() {
  if (_ui->source.starts_with("http")) {
    _tileLoader =
        std::make_shared<TileLoaderUrlMap>(_ui->source, _ui->requestLimit);
  } else if (!_ui->source.empty()) {
    _tileLoader =
        std::make_shared<TileLoaderFsMap>(_ui->source, _ui->requestLimit);
  }
  _mapPlot->setTileLoader(_tileLoader);
}

} // namespace ImOsm
