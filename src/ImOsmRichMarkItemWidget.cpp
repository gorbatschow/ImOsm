#include "ImOsmRichMarkItemWidget.h"

namespace ImOsm {

inline static constexpr const char* MarkerTypeName(ImPlotMarker marker);

RichMarkItemWidget::RichMarkItemWidget(std::shared_ptr<RichMarkItem> item,
                                       const GeoCoords& latLonPicked)
    : _item{item}
    , _pickedCoords{latLonPicked} {
  _text = _item->text();
  _latLon[0] = _item->geoCoords().lat;
  _latLon[1] = _item->geoCoords().lon;
  _radius = _item->radius();
  _textEnabled = _item->style().textEnabled;
  _markerEnabled = _item->style().markerEnabled;
  _radiusEnabled = _item->style().radiusEnabled;
  _radiusWeight = _item->style().radiusWeight;
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
  ImGui::InputFloat2("Lat/Lon [deg]", _latLon.data(), _latLonFormat);
  ImGui::SameLine();
  paint_pickedBtn();
  ImGui::InputFloat("Radius [m]", &_radius, {}, {}, _radiusFormat);
  ImGui::Separator();
  ImGui::TextUnformatted("Style");
  ImGui::Checkbox("Text Enabled", &_textEnabled);
  ImGui::SameLine();
  ImGui::Checkbox("Marker Enabled", &_markerEnabled);
  ImGui::SameLine();
  ImGui::Checkbox("Radius Enabled", &_radiusEnabled);
  ImGui::InputFloat("Radius Weight", &_radiusWeight, 1.0f);
  paint_markerCombo();
  ImGui::InputFloat("Marker Size", &_markerSize, 1.0f);
  //ImGui::InputFloat("Marker Weight", &_markerWeight, 1.0f);
  ImGui::ColorEdit3("Marker Fill", _markerFill.data());
}

void RichMarkItemWidget::apply() {
  _item->setText(_text);
  _item->setCoords({_latLon[0], _latLon[1]});
  _item->setRadius(_radius);

  _item->style().textEnabled = _textEnabled;
  _item->style().markerEnabled = _markerEnabled;
  _item->style().radiusEnabled = _radiusEnabled;

  _item->style().radiusWeight = _radiusWeight;
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
    _latLon = {float(_pickedCoords.lat), float(_pickedCoords.lon)};
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
