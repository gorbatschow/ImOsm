#pragma once
#include "ImOsmCoords.h"
#include "ImOsmIRichItem.h"
#include <string>
#include <vector>

namespace ImOsm {
namespace Rich {
class MarkItem : public IRichItem
{
public:
  struct Style {
    //using ImPlotMarker = int;
    bool textEnabled{true};
    bool markerEnabled{true};
    bool radiusEnabled{true};
    ImPlotMarker markerType{ImPlotMarker_Circle};
    float markerSize{10.f};
    float markerWeight{};
    ImVec4 markerFill{1.f, 0.f, 0.f, 1.f};
    ImVec4 markerOutline{};
    float radiusWeight{2.f};
  };

  MarkItem();
  MarkItem(const GeoCoords &coords, const std::string &text);
  virtual ~MarkItem() override;

  virtual bool inBounds(float minLat,
                        float maxLat,
                        float minLon,
                        float maxLon) const override;

  virtual void setEnabled(bool enabled) override { _enabled = enabled; }
  virtual bool enabled() const override { return _enabled; }

  virtual void paint() override;

  inline void setCoords(const GeoCoords &coords) {
    _geoCoords = coords;
    _osmCoords = _geoCoords.toOsmCoords();
    if (_r > 0.0) {
      updateRadiusPoints();
      updateRadiusBounds();
    }
  }

  inline const GeoCoords &geoCoords() const { return _geoCoords; }
  inline const OsmCoords &osmCoords() const { return _osmCoords; }

  inline void setRadius(float r) {
    _r = r;
    updateRadiusPoints();
    updateRadiusBounds();
  }
  inline float radius() const { return _r; }

  inline void setText(const std::string &text) { _text = text; }
  inline const std::string &text() const { return _text; }

  inline void setStyle(const Style &style) { _style = style; }
  inline const Style &style() const { return _style; }
  inline Style &style() { return _style; }

private:
  void updateRadiusPoints();
  void updateRadiusBounds();

private:
  bool _enabled{true};
  GeoCoords _geoCoords{};
  OsmCoords _osmCoords{};
  std::string _text;
  double _latTL{}, _latTR{}, _latBL{}, _latBR{};
  double _lonTL{}, _lonTR{}, _lonBL{}, _lonBR{};
  float _r{};
  std::vector<float> _rx, _ry;
  float _dphi{1.0};
  Style _style;
};
} // namespace Rich
} // namespace ImOsm
