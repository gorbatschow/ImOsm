#pragma once
#include "ImOsmRichMarkItem.h"
#include <array>
#include <memory>
#include <misc/cpp/imgui_stdlib.h>

namespace ImOsm {
class RichMarkItemWidget
{
public:
  RichMarkItemWidget(std::shared_ptr<RichMarkItem> item,
                     std::array<float, 2> latLonPicked = {});
  ~RichMarkItemWidget() = default;
  void paint();
  void apply();

private:
  void paint_pickedBtn();
  void paint_markerCombo();

private:
  std::shared_ptr<RichMarkItem> _item;
  std::array<float, 2> _latLonPicked{};

  inline static const char _latLonFormat[]{"%.6f"};
  std::array<float, 2> _latLon{};
  std::string _text;
  std::string _markerTypeName;
  ImPlotMarker _markerType;
  bool _textEnabled{}, _markerEnabled{};
  float _markerSize{}, _markerWeight{};
  std::array<float, 3> _markerFill{};
};

} // namespace ImOsm
