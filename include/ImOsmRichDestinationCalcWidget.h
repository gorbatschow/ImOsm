#pragma once
#include "ImOsmRichMarkStorage.h"
#include <memory>
#include <misc/cpp/imgui_stdlib.h>

namespace ImOsm {
class RichDestinationCalcWidget
{
public:
  RichDestinationCalcWidget(std::shared_ptr<RichMarkStorage> storage)
      : _storage{storage} {}

  void paint() {
    ImGui::PushID(this);
    ImGui::TextUnformatted("Destination Calculator");
    ImGui::SetNextItemWidth(100.f);
    ImGui::InputText("Mark A", &_markNameA);
    ImGui::SetNextItemWidth(100.f);
    ImGui::InputFloat("Distance [m]##", &_distance, {}, {}, "%.2f");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100.f);
    ImGui::InputFloat("Bearing [deg]##", &_bearing, {}, {}, "%.2f");
    ImGui::SameLine();
    if (ImGui::Button("Calculate##")) {
      bool foundA{false};
      _markA = _storage->findMark(_markNameA, foundA);
      if (foundA) {
        _markB = _markA.destination(_distance, _bearing);
      }
    }
    ImGui::Text("Destination [deg] Lat: %.6f Lon: %.6f", _markB.lat, _markB.lon);
    ImGui::SameLine();
    if (ImGui::Button("Pick##")) {
      _storage->setPickCoords(_markB);
    }
    ImGui::PopID();
  }

private:
  std::shared_ptr<RichMarkStorage> _storage;
  std::string _markNameA;
  GeoCoords _markA, _markB;
  float _distance{}, _bearing{};
};
} // namespace ImOsm
