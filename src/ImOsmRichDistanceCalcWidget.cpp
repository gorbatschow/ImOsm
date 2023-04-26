#include "ImOsmRichDistanceCalcWidget.h"
#include "ImOsmRichMarkStorage.h"
#include <misc/cpp/imgui_stdlib.h>

namespace ImOsm {
namespace Rich {

struct DistanceCalcWidget::Ui {
  std::string markNameA, markNameB;
  GeoCoords markA, markB, markC;
  double distance{}, bearing{};
};

DistanceCalcWidget::DistanceCalcWidget(std::shared_ptr<MarkStorage> storage)
    : _storage{storage}, _ui{std::make_unique<DistanceCalcWidget::Ui>()} {}

DistanceCalcWidget::~DistanceCalcWidget() = default;

void DistanceCalcWidget::paint() {
  ImGui::PushID(this);
  ImGui::TextUnformatted("D/B/M Calculator");
  ImGui::SetNextItemWidth(100.f);
  ImGui::InputText("Mark A##", &_ui->markNameA);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(100.f);
  ImGui::InputText("Mark B##", &_ui->markNameB);
  ImGui::SameLine();
  if (ImGui::Button("Calculate##")) {
    bool foundA{false}, foundB{false};
    _ui->markA = _storage->findMark(_ui->markNameA, foundA);
    _ui->markB = _storage->findMark(_ui->markNameB, foundB);
    if (foundA && foundB) {
      _ui->distance = _ui->markA.distance(_ui->markB);
      _ui->bearing = _ui->markA.bearing(_ui->markB);
      _ui->markC = _ui->markA.midpoint(_ui->markB);
    }
  }
  if (_ui->distance < 1e3) {
    ImGui::Text("Distance [m] %.2f", _ui->distance);
  } else {
    ImGui::Text("Distance [km] %.2f", _ui->distance * 1e-3);
  }
  ImGui::Text("Bearing [deg] %.2f", _ui->bearing);
  ImGui::Text("Midpoint [deg] Lat: %.6f Lon: %.6f", _ui->markC.lat,
              _ui->markC.lon);
  ImGui::SameLine();
  if (ImGui::Button("Pick##")) {
    _storage->setPickCoords(_ui->markC);
  }
  ImGui::PopID();
}
} // namespace Rich
} // namespace ImOsm
