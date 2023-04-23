#include <ImOsmRichMarkItem.h>

namespace ImOsm {
namespace Rich {
MarkItem::MarkItem() {
  _osmCoords = _geoCoords.toOsmCoords();
  _rx.resize(int(360.0 / _dphi) + 1);
  _ry.resize(int(360.0 / _dphi) + 1);
}

MarkItem::MarkItem(const GeoCoords &coords, const std::string &text)
    : _geoCoords{coords}
    , _text{text} {
  _osmCoords = _geoCoords.toOsmCoords();
  _rx.resize(int(360.0 / _dphi) + 1);
  _ry.resize(int(360.0 / _dphi) + 1);
}

MarkItem::~MarkItem() = default;

bool MarkItem::inBounds(float minLat,
                        float maxLat,
                        float minLon,
                        float maxLon) const {
  /*
  const auto check{[minLat, maxLat, minLon, maxLon](double lat, double lon) {
    return lat > minLat && lat < maxLat && lon > minLon && lon < maxLon;
  }};

  if (_r < std::numeric_limits<decltype(_r)>::epsilon()) {
    return check(_lat, _lon);
  } */
  return true;
}

void MarkItem::paint() {
  if (_style.markerEnabled) {
    ImPlot::SetNextMarkerStyle(_style.markerType,
                               _style.markerSize,
                               _style.markerFill,
                               _style.markerWeight,
                               _style.markerOutline);
    ImGui::PushID(&_osmCoords);
    ImPlot::PlotScatter("##", &_osmCoords.x, &_osmCoords.y, 1);
    ImGui::PopID();
  }

  if (_style.textEnabled) {
    ImGui::PushStyleColor(ImGuiCol_Text, _style.markerFill);
    ImGui::PushID(_text.c_str());
    ImPlot::PlotText(_text.c_str(),
                     _osmCoords.x,
                     _osmCoords.y,
                     {0.f, _style.markerSize + ImGui::GetFontSize()});
    ImGui::PopID();
    ImGui::PopStyleColor();
  }

  if (_style.radiusEnabled) {
    ImPlot::SetNextLineStyle(_style.markerFill, _style.radiusWeight);
    ImGui::PushID(_rx.data());
    ImPlot::PlotLine("##", _rx.data(), _ry.data(), _rx.size());
    ImGui::PopID();
  }
}

void MarkItem::updateRadiusPoints() {
  /*
  double lat{}, lon{};
  double x{}, y{};
  double phi{};
  for (int i{}; i != _rx.size(); ++i, phi += _dphi) {
    LatLon::destination(lat, lon, _lat, _lon, _r, phi);
    _rx[i] = lon2x(lon);
    _ry[i] = lat2y(lat);
  } */

  double lat{}, lon{};
  LatLon::destination(lat, lon, _geoCoords.lat, _geoCoords.lon, _r, {});
  const double x{lon2x(lon)}, y{lat2y(lat)};
  const double r{
      sqrt(std::pow(x - _osmCoords.x, 2.0) + std::pow(y - _osmCoords.y, 2.0))};
  const double dphi{_dphi * LatLon::RAD};
  const double x0{x}, y0{y + r};
  double phi{};
  for (int i{}; i != _rx.size(); ++i, phi += dphi) {
    _rx[i] = x0 + r * cos(phi);
    _ry[i] = y0 + r * sin(phi);
  }
}

void MarkItem::updateRadiusBounds() {
  LatLon::destination(_latTR, _lonTR, _geoCoords.lat, _geoCoords.lon, _r, 45.0);
  LatLon::destination(_latBR, _lonBR, _geoCoords.lat, _geoCoords.lon, _r, 135.0);
  LatLon::destination(_latBL, _lonBL, _geoCoords.lat, _geoCoords.lon, _r, 225.0);
  LatLon::destination(_latTL, _lonTL, _geoCoords.lat, _geoCoords.lon, _r, 315.0);
}
} // namespace Rich
} // namespace ImOsm
