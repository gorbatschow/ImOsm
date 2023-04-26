#include "ImOsmRichDestinationCalcWidget.h"
#include "ImOsmCoords.h"
#include "ImOsmRichMarkStorage.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace ImOsm {
namespace Rich {

struct DestinationCalcWidget::Ui {
  std::string markNameA;
  GeoCoords markA, markB;
  float distance{}, bearing{};
};

DestinationCalcWidget::DestinationCalcWidget(
    std::shared_ptr<MarkStorage> storage)
    : _storage{storage}, _ui{std::make_unique<Ui>()} {}

DestinationCalcWidget::~DestinationCalcWidget() = default;

void DestinationCalcWidget::paint() {
  ImGui::PushID(this);
  ImGui::TextUnformatted("Destination Calculator");
  ImGui::SetNextItemWidth(100.f);
  ImGui::InputText("Mark A", &_ui->markNameA);
  ImGui::SetNextItemWidth(100.f);
  ImGui::InputFloat("Distance [m]##", &_ui->distance, {}, {}, "%.2f");
  ImGui::SameLine();
  ImGui::SetNextItemWidth(100.f);
  ImGui::InputFloat("Bearing [deg]##", &_ui->bearing, {}, {}, "%.2f");
  ImGui::SameLine();
  if (ImGui::Button("Calculate##")) {
    bool foundA{false};
    _ui->markA = _storage->findMark(_ui->markNameA, foundA);
    if (foundA) {
      _ui->markB = _ui->markA.destination(_ui->distance, _ui->bearing);
    }
  }

  ImGui::Text("Destination [deg] Lat: %.6f Lon: %.6f", _ui->markB.lat,
              _ui->markB.lon);
  ImGui::SameLine();
  if (ImGui::Button("Pick##")) {
    _storage->setPickCoords(_ui->markB);
  }
  ImGui::PopID();
}
} // namespace Rich
} // namespace ImOsm
