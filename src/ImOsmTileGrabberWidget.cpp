#include "ImOsmTileGrabberWidget.h"
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
  _ui->minZ = std::clamp(_ui->minZ, 0, MaxZoom);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(100);
  ImGui::InputInt("Max Z", &_ui->maxZ);
  _ui->maxZ = std::clamp(_ui->maxZ, 0, MaxZoom);
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
