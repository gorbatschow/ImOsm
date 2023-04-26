#pragma once
#include "ImOsmCoords.h"
#include <memory>

namespace ImOsm {
namespace Rich {
class MarkItem;

class MarkItemWidget {
public:
  MarkItemWidget(std::shared_ptr<MarkItem> item, const GeoCoords &pickedCoords);
  ~MarkItemWidget();

  void paint();
  void apply();

private:
  void paint_pickedBtn();
  void paint_markerCombo();

private:
  std::shared_ptr<MarkItem> _item;
  GeoCoords _pickedCoords{};

  struct Ui;
  std::unique_ptr<Ui> _ui;
};
} // namespace Rich
} // namespace ImOsm
