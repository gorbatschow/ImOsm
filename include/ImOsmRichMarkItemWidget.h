#pragma once
#include "ImOsmRichMarkItem.h"
#include <array>
#include <memory>
#include <misc/cpp/imgui_stdlib.h>

namespace ImOsm {
namespace Rich {
class MarkItemWidget
{
public:
  MarkItemWidget(std::shared_ptr<MarkItem> item, const GeoCoords &pickedCoords);
  ~MarkItemWidget() = default;
  void paint();
  void apply();

private:
  void paint_pickedBtn();
  void paint_markerCombo();

private:
  std::shared_ptr<MarkItem> _item;
  GeoCoords _pickedCoords{};

  inline static constexpr char _latLonFormat[]{"%.6f"};
  std::array<float, 2> _latLon{};
  inline static constexpr char _radiusFormat[]{"%.0f"};
  float _radius{};
  std::string _text;
  std::string _markerTypeName;
  ImPlotMarker _markerType;
  bool _textEnabled{}, _markerEnabled{}, _radiusEnabled;
  float _markerSize{}, _markerWeight{}, _radiusWeight{};
  std::array<float, 3> _markerFill{};
};
} // namespace Rich
} // namespace ImOsm
