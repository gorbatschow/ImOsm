#include "ImOsmTileGrabberWidget.h"
#include "ImOsmCoords.h"
#include "ImOsmMapPlot.h"
#include "ImOsmTileGrabber.h"
#include "ImOsmTileSaver.h"
#include "ImOsmTileSourceUrlImpl.h"
#include <cmath>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <string>

namespace ImOsm {
struct TileGrabberWidget::Ui {
  std::string url{};
  std::string dirname{"tiles"};
  int requestLimit{10};
  const bool preload{false};
  int minZ{0}, maxZ{18};
  int tileCount{0}, tileTotal{};
  float progress{};
};

TileGrabberWidget::TileGrabberWidget(std::shared_ptr<MapPlot> mapPlot)
    : _mapPlot{mapPlot}, _ui{std::make_unique<Ui>()} {
  _ui->url = TileSourceUrlOsm::URL_TPL;
}

void TileGrabberWidget::loadState(const mINI::INIStructure &ini) {
  if (ini.has("tile_grabber")) {
    const auto opts{ini.get("tile_grabber")};
    if (opts.has("source_url")) {
      _ui->url = opts.get("source_url");
    }
    if (opts.has("target_dir")) {
      _ui->dirname = opts.get("target_dir");
    }
    if (opts.has("request_limit")) {
      _ui->requestLimit = std::stoi(opts.get("request_limit"));
      std::clamp(_ui->requestLimit, 1, 99);
    }
    if (opts.has("min_zoom")) {
      _ui->minZ = std::stoi(opts.get("min_zoom"));
      std::clamp(_ui->maxZ, MIN_ZOOM, MAX_ZOOM);
    }
    if (opts.has("max_zoom")) {
      _ui->maxZ = std::stoi(opts.get("max_zoom"));
      std::clamp(_ui->maxZ, MIN_ZOOM, MAX_ZOOM);
    }
  }
}

void TileGrabberWidget::saveState(mINI::INIStructure &ini) const {
  ini["tile_grabber"].set("source_url", _ui->url);
  ini["tile_grabber"].set("target_dir", _ui->dirname);
  ini["tile_grabber"].set("request_limit", std::to_string(_ui->requestLimit));
  ini["tile_grabber"].set("min_zoom", std::to_string(_ui->minZ));
  ini["tile_grabber"].set("max_zoom", std::to_string(_ui->maxZ));
}

TileGrabberWidget::~TileGrabberWidget() = default;

void ImOsm::TileGrabberWidget::paint() {
  ImGui::PushID(this);
  ImGui::TextUnformatted("Tile Grabber");
  ImGui::Text("Min Lat %.6f, Min Lon %.6f", _mapPlot->minLat(),
              _mapPlot->minLon());
  ImGui::Text("Max Lat %.6f, Max Lon %.6f", _mapPlot->maxLat(),
              _mapPlot->maxLon());
  ImGui::AlignTextToFramePadding();

  ImGui::SetNextItemWidth(100);
  ImGui::InputInt("Min Z", &_ui->minZ);
  _ui->minZ = std::clamp(_ui->minZ, 0, MAX_ZOOM);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(100);
  ImGui::InputInt("Max Z", &_ui->maxZ);
  _ui->maxZ = std::clamp(_ui->maxZ, 0, MAX_ZOOM);
  ImGui::SameLine();
  ImGui::Text("Tiles Count: %lu",
              countTiles(_mapPlot->minLat(), _mapPlot->maxLat(),
                         _mapPlot->minLon(), _mapPlot->maxLon(), _ui->minZ,
                         _ui->maxZ));

  ImGui::InputText("Tile Server URL##", &_ui->url);

  ImGui::SetNextItemWidth(100);
  ImGui::InputInt("Max. Requests", &_ui->requestLimit);
  _ui->requestLimit = std::clamp(_ui->requestLimit, 1, 99);
  ImGui::SameLine();
  if (ImGui::Button("OSM")) {
    _ui->url = TileSourceUrlOsm::URL_TPL;
  };
  ImGui::SameLine();
  if (ImGui::Button("ARC")) {
    _ui->url = TileSourceUrlArcImagery::URL_TPL;
  };

  ImGui::InputText("Destination Dir##", &_ui->dirname);
  if (ImGui::Button("Grab")) {
    _ui->tileTotal =
        countTiles(_mapPlot->minLat(), _mapPlot->maxLat(), _mapPlot->minLon(),
                   _mapPlot->maxLon(), _ui->minZ, _ui->maxZ);
    _tileGrabber = std::make_unique<TileGrabber>(
        std::make_shared<TileSourceUrlCustom>(_ui->requestLimit, _ui->preload,
                                              _ui->url),
        std::make_shared<TileSaverSubDir>(_ui->dirname));
    _tileGrabber->grab(_mapPlot->minLat(), _mapPlot->maxLat(),
                       _mapPlot->minLon(), _mapPlot->maxLon(), _ui->minZ,
                       _ui->maxZ);
  }
  ImGui::SameLine();
  if (ImGui::Button("Stop") && _tileGrabber) {
    _tileGrabber->stop();
  }

  if (_tileGrabber) {
    _ui->tileCount = _tileGrabber->tileCounter();
  }
  _ui->progress = float(_ui->tileCount) / float(_ui->tileTotal);
  _ui->progress = std::isnan(_ui->progress) ? 0.f : _ui->progress;
  ImGui::SameLine();
  ImGui::Text("%d/%d", _ui->tileCount, _ui->tileTotal);
  ImGui::SameLine();
  ImGui::ProgressBar(_ui->progress);

  ImGui::PopID();
}
} // namespace ImOsm
