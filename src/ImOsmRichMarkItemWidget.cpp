#include "ImOsmRichMarkItemWidget.h"

namespace ImOsm {

inline static constexpr const char* MarkerTypeName(ImPlotMarker marker);

RichMarkItemWidget::RichMarkItemWidget(std::shared_ptr<RichMarkItem> item,
                                       std::array<float, 2> latLonPicked)
    : _item{item}
    , _latLonPicked{latLonPicked} {
  _text = _item->text();
  _latLon[0] = _item->lat();
  _latLon[1] = _item->lon();
  _textEnabled = _item->style().textEnabled;
  _markerEnabled = _item->style().markerEnabled;
  _markerType = _item->style().markerType;
  _markerTypeName = MarkerTypeName(_markerType);
  _markerSize = _item->style().markerSize;
  _markerWeight = _item->style().markerWeight;
  _markerFill = {_item->style().markerFill.x,
                 _item->style().markerFill.y,
                 _item->style().markerFill.z};
}

void RichMarkItemWidget::paint() {
  ImGui::InputText("Name", &_text);
  ImGui::InputFloat2("Lat/Lon", _latLon.data(), _latLonFormat);
  ImGui::SameLine();
  paint_pickedBtn();
  ImGui::Separator();
  ImGui::TextUnformatted("Style");
  ImGui::Checkbox("Text Enabled", &_textEnabled);
  ImGui::SameLine();
  ImGui::Checkbox("Marker Enabled", &_markerEnabled);
  paint_markerCombo();
  ImGui::InputFloat("Marker Size", &_markerSize, 1.0f);
  //ImGui::InputFloat("Marker Weight", &_markerWeight, 1.0f);
  ImGui::ColorEdit3("Marker Fill", _markerFill.data());
}

void RichMarkItemWidget::apply() {
  _item->setText(_text);
  _item->setLatLon(_latLon[0], _latLon[1]);
  _item->style().textEnabled = _textEnabled;
  _item->style().markerEnabled = _markerEnabled;
  _item->style().markerType = _markerType;
  _item->style().markerSize = _markerSize;
  _item->style().markerWeight = _markerWeight;
  _item->style().markerFill = {_markerFill[0],
                               _markerFill[1],
                               _markerFill[2],
                               1.0};
}

void RichMarkItemWidget::paint_pickedBtn() {
  if (ImGui::Button("Picked")) {
    _latLon = _latLonPicked;
  }
}

void RichMarkItemWidget::paint_markerCombo() {
  if (ImGui::BeginCombo("Marker", _markerTypeName.c_str())) {
    if (ImGui::Selectable(MarkerTypeName(ImPlotMarker_Circle),
                          _markerType == ImPlotMarker_Circle)) {
      _markerType = ImPlotMarker_Circle;
      _markerTypeName = MarkerTypeName(ImPlotMarker_Circle);
    } else if (ImGui::Selectable(MarkerTypeName(ImPlotMarker_Square),
                                 _markerType == ImPlotMarker_Square)) {
      _markerType = ImPlotMarker_Square;
      _markerTypeName = MarkerTypeName(ImPlotMarker_Square);
    } else if (ImGui::Selectable(MarkerTypeName(ImPlotMarker_Diamond),
                                 _markerType == ImPlotMarker_Diamond)) {
      _markerType = ImPlotMarker_Diamond;
      _markerTypeName = MarkerTypeName(ImPlotMarker_Diamond);
    } else if (ImGui::Selectable(MarkerTypeName(ImPlotMarker_Up),
                                 _markerType == ImPlotMarker_Up)) {
      _markerType = ImPlotMarker_Up;
      _markerTypeName = MarkerTypeName(ImPlotMarker_Up);
    }
    ImGui::EndCombo();
  }
}

inline static constexpr const char* MarkerTypeName(ImPlotMarker marker) {
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

} // namespace ImOsm
