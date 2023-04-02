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
    bool textEnabled{true};
    bool markerEnabled{true};
    ImPlotMarker markerType{ImPlotMarker_Circle};
    float markerSize{10.f};
    float markerWeight{};
    ImVec4 markerFill{1.f, 1.f, 1.f, 1.f};
    ImVec4 markerOutline{};
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
    if (_style.textEnabled) {
      ImPlot::PlotText(_text.c_str(),
                       _x,
                       _y,
                       {_style.markerSize, _style.markerSize});
    }

    if (_style.markerEnabled) {
      ImPlot::SetNextMarkerStyle(_style.markerType,
                                 _style.markerSize,
                                 _style.markerFill,
                                 _style.markerWeight,
                                 _style.markerOutline);
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
