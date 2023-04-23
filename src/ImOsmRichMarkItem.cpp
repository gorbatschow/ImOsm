#include <ImOsmRichMarkItem.h>

ImOsm::RichMarkItem::RichMarkItem() {
  _rx.resize(int(360.0 / _dphi) + 1);
  _ry.resize(int(360.0 / _dphi) + 1);
}

ImOsm::RichMarkItem::RichMarkItem(float lat, float lon, const std::string &text)
    : _lat{lat}
    , _lon{lon}
    , _text{text} {
  _x = lon2x(_lon, 0);
  _y = lat2y(_lat, 0);
  _rx.resize(int(360.0 / _dphi) + 1);
  _ry.resize(int(360.0 / _dphi) + 1);
}

ImOsm::RichMarkItem::~RichMarkItem() = default;

bool ImOsm::RichMarkItem::inBounds(float minLat,
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

void ImOsm::RichMarkItem::paint() {
  if (_style.markerEnabled) {
    ImPlot::SetNextMarkerStyle(_style.markerType,
                               _style.markerSize,
                               _style.markerFill,
                               _style.markerWeight,
                               _style.markerOutline);
    ImGui::PushID(&_x);
    ImPlot::PlotScatter("##", &_x, &_y, 1);
    ImGui::PopID();
  }

  if (_style.textEnabled) {
    ImGui::PushStyleColor(ImGuiCol_Text, _style.markerFill);
    ImGui::PushID(_text.c_str());
    ImPlot::PlotText(_text.c_str(),
                     _x,
                     _y,
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

void ImOsm::RichMarkItem::updateRadiusPoints() {
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
  LatLon::destination(lat, lon, _lat, _lon, _r, {});
  const double x{lon2x(lon)}, y{lat2y(lat)};
  const double r{sqrt(std::pow(x - _x, 2.0) + std::pow(y - _y, 2.0))};
  const double dphi{_dphi * LatLon::RAD};
  const double x0{x}, y0{y + r};
  double phi{};
  for (int i{}; i != _rx.size(); ++i, phi += dphi) {
    _rx[i] = x0 + r * cos(phi);
    _ry[i] = y0 + r * sin(phi);
  }
}

void ImOsm::RichMarkItem::updateRadiusBounds() {
  LatLon::destination(_latTR, _lonTR, _lat, _lon, _r, 45.0);
  LatLon::destination(_latBR, _lonBR, _lat, _lon, _r, 135.0);
  LatLon::destination(_latBL, _lonBL, _lat, _lon, _r, 225.0);
  LatLon::destination(_latTL, _lonTL, _lat, _lon, _r, 315.0);
}
