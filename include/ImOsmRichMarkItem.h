#pragma once
#include "ImOsmCoords.h"
#include "ImOsmIRichItem.h"
#include <implot.h>
#include <string>

namespace ImOsm {
class RichMarkItem : public IRichItem
{
public:
  struct Style
  {
    ImPlotMarker marker{ImPlotMarker_Circle};
    float size{10.f};
    float weight{};
    ImVec4 fill{1.f, 1.f, 1.f, 1.f};
    ImVec4 outline{};
  };

  RichMarkItem(float lat, float lon, const std::string &text)
      : _lat{lat}
      , _lon{lon}
      , _text{text} {
    _x = lon2x(_lon, 0);
    _y = lat2y(_lat, 0);
  }

  virtual ~RichMarkItem() = default;

  virtual bool inBounds(float minLat,
                        float maxLat,
                        float minLon,
                        float maxLon) const override {
    return _lat > minLat && _lat < maxLat && _lon > minLon && _lon < maxLon;
  }

  virtual void paint() override {
    if (!_text.empty()) {
      ImPlot::PlotText(_text.c_str(), _x, _y);
      ImPlot::SetNextMarkerStyle(_style.marker,
                                 _style.size,
                                 _style.fill,
                                 _style.weight,
                                 _style.outline);
      ImPlot::PlotScatter("##", &_x, &_y, 1);
    }
  }

  inline void setLatLon(float lat, float lon) {
    _lat = lat;
    _lon = lon;
    _x = lon2x(_lon, 0);
    _y = lat2y(_lat, 0);
  }

  inline float lat() const { return _lat; }
  inline float lon() const { return _lon; }

  inline void setText(const std::string &text) { _text = text; }
  inline const std::string &text() const { return _text; }

  inline void setStyle(const Style &style) { _style = style; }
  inline const Style &style() const { return _style; }
  inline Style &style() { return _style; }

private:
  std::string _text;
  float _lat{}, _lon{};
  float _x{}, _y{};
  Style _style;
};
} // namespace ImOsm
