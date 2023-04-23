#pragma once
#include "ImOsmRichMarkStorage.h"
#include <imgui.h>
#include <memory>
#include <misc/cpp/imgui_stdlib.h>
#include <string>

namespace ImOsm {
class RichDistanceCalcWidget
{
public:
  RichDistanceCalcWidget(std::shared_ptr<RichMarkStorage> storage)
      : _storage{storage} {}

  void paint() {
    ImGui::PushID(this);
    ImGui::TextUnformatted("D/B/M Calculator");
    ImGui::SetNextItemWidth(100.f);
    ImGui::InputText("Mark A##", &_markNameA);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100.f);
    ImGui::InputText("Mark B##", &_markNameB);
    ImGui::SameLine();
    if (ImGui::Button("Calculate##")) {
      bool foundA{false}, foundB{false};
      _markA = _storage->findMark(_markNameA, foundA);
      _markB = _storage->findMark(_markNameB, foundB);
      if (foundA && foundB) {
        _distance = _markA.distance(_markB);
        _bearing = _markA.bearing(_markB);
        _markC = _markA.midpoint(_markB);
      }
    }
    if (_distance < 1e3) {
      ImGui::Text("Distance [m] %.2f", _distance);
    } else {
      ImGui::Text("Distance [km] %.2f", _distance * 1e-3);
    }
    ImGui::Text("Bearing [deg] %.2f", _bearing);
    ImGui::Text("Midpoint [deg] Lat: %.6f Lon: %.6f", _markC.lat, _markC.lon);
    ImGui::SameLine();
    if (ImGui::Button("Pick##")) {
      _storage->setPickCoords(_markC);
    }
    ImGui::PopID();
  }

private:
  std::string _markNameA, _markNameB;
  GeoCoords _markA, _markB, _markC;
  std::shared_ptr<RichMarkStorage> _storage;
  double _distance{}, _bearing{};
};
} // namespace ImOsm
