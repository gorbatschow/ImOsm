#include "ImOsmRichMarkItemWidget.h"
#include "ImOsmRichMarkItem.h"
#include <misc/cpp/imgui_stdlib.h>

namespace ImOsm {
namespace Rich {

struct MarkItemWidget::Ui {
  inline static constexpr char latLonFormat[]{"%.6f"};
  std::array<float, 2> latLon{};
  inline static constexpr char radiusFormat[]{"%.0f"};
  float radius{};
  std::string text;
  std::string markerTypeName;
  ImPlotMarker markerType;
  bool textEnabled{}, markerEnabled{}, radiusEnabled;
  float markerSize{}, markerWeight{}, radiusWeight{};
  std::array<float, 3> markerFill{};
};

inline static constexpr const char *MarkerTypeName(ImPlotMarker marker);

MarkItemWidget::MarkItemWidget(std::shared_ptr<MarkItem> item,
                               const GeoCoords &latLonPicked)
    : _item{item}, _pickedCoords{latLonPicked} {
  _ui->text = _item->text();
  _ui->latLon[0] = _item->geoCoords().lat;
  _ui->latLon[1] = _item->geoCoords().lon;
  _ui->radius = _item->radius();
  _ui->textEnabled = _item->style().textEnabled;
  _ui->markerEnabled = _item->style().markerEnabled;
  _ui->radiusEnabled = _item->style().radiusEnabled;
  _ui->radiusWeight = _item->style().radiusWeight;
  _ui->markerType = _item->style().markerType;
  _ui->markerTypeName = MarkerTypeName(_ui->markerType);
  _ui->markerSize = _item->style().markerSize;
  _ui->markerWeight = _item->style().markerWeight;
  _ui->markerFill = {_item->style().markerFill.x, _item->style().markerFill.y,
                     _item->style().markerFill.z};
}

MarkItemWidget::~MarkItemWidget() = default;

void MarkItemWidget::paint() {
  ImGui::InputText("Name", &_ui->text);
  ImGui::InputFloat2("Lat/Lon [deg]", _ui->latLon.data(), _ui->latLonFormat);
  ImGui::SameLine();
  paint_pickedBtn();
  ImGui::InputFloat("Radius [m]", &_ui->radius, {}, {}, _ui->radiusFormat);
  ImGui::Separator();
  ImGui::TextUnformatted("Style");
  ImGui::Checkbox("Text Enabled", &_ui->textEnabled);
  ImGui::SameLine();
  ImGui::Checkbox("Marker Enabled", &_ui->markerEnabled);
  ImGui::SameLine();
  ImGui::Checkbox("Radius Enabled", &_ui->radiusEnabled);
  ImGui::InputFloat("Radius Weight", &_ui->radiusWeight, 1.0f);
  paint_markerCombo();
  ImGui::InputFloat("Marker Size", &_ui->markerSize, 1.0f);
  // ImGui::InputFloat("Marker Weight", &_markerWeight, 1.0f);
  ImGui::ColorEdit3("Marker Fill", _ui->markerFill.data());
}

void MarkItemWidget::apply() {
  _item->setText(_ui->text);
  _item->setCoords({_ui->latLon[0], _ui->latLon[1]});
  _item->setRadius(_ui->radius);

  _item->style().textEnabled = _ui->textEnabled;
  _item->style().markerEnabled = _ui->markerEnabled;
  _item->style().radiusEnabled = _ui->radiusEnabled;

  _item->style().radiusWeight = _ui->radiusWeight;
  _item->style().markerType = _ui->markerType;
  _item->style().markerSize = _ui->markerSize;
  _item->style().markerWeight = _ui->markerWeight;
  _item->style().markerFill = {_ui->markerFill[0], _ui->markerFill[1],
                               _ui->markerFill[2], 1.0};
}

void MarkItemWidget::paint_pickedBtn() {
  if (ImGui::Button("Picked")) {
    _ui->latLon = {float(_pickedCoords.lat), float(_pickedCoords.lon)};
  }
}

void MarkItemWidget::paint_markerCombo() {
  if (ImGui::BeginCombo("Marker", _ui->markerTypeName.c_str())) {
    if (ImGui::Selectable(MarkerTypeName(ImPlotMarker_Circle),
                          _ui->markerType == ImPlotMarker_Circle)) {
      _ui->markerType = ImPlotMarker_Circle;
      _ui->markerTypeName = MarkerTypeName(ImPlotMarker_Circle);
    } else if (ImGui::Selectable(MarkerTypeName(ImPlotMarker_Square),
                                 _ui->markerType == ImPlotMarker_Square)) {
      _ui->markerType = ImPlotMarker_Square;
      _ui->markerTypeName = MarkerTypeName(ImPlotMarker_Square);
    } else if (ImGui::Selectable(MarkerTypeName(ImPlotMarker_Diamond),
                                 _ui->markerType == ImPlotMarker_Diamond)) {
      _ui->markerType = ImPlotMarker_Diamond;
      _ui->markerTypeName = MarkerTypeName(ImPlotMarker_Diamond);
    } else if (ImGui::Selectable(MarkerTypeName(ImPlotMarker_Up),
                                 _ui->markerType == ImPlotMarker_Up)) {
      _ui->markerType = ImPlotMarker_Up;
      _ui->markerTypeName = MarkerTypeName(ImPlotMarker_Up);
    }
    ImGui::EndCombo();
  }
}

inline static constexpr const char *MarkerTypeName(ImPlotMarker marker) {
  if (marker == ImPlotMarker_Circle) {
    return "Circle";
  } else if (marker == ImPlotMarker_Square) {
    return "Square";
  } else if (marker == ImPlotMarker_Diamond) {
    return "Diamond";
  } else if (marker == ImPlotMarker_Up) {
    return "Triangle UP";
  }
  return "Unknow";
}
} // namespace Rich
} // namespace ImOsm
