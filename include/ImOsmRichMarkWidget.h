#pragma once
#include "ImOsmRichMapPlot.h"
#include "ImOsmRichMarkItem.h"
#include "ImOsmRichMarkItemWidget.h"
#include <array>
#include <imgui.h>

namespace ImOsm {
class RichMarkWidget
{
public:
  RichMarkWidget(std::shared_ptr<RichMapPlot> plot);
  virtual ~RichMarkWidget() = default;

  void paint();

private:
  void paint_latLonInput();
  void paint_mousePickBtn();
  void paint_markNameInput();
  void paint_addMarkBtn();
  void paint_markTable();
  void paint_markTableRow(std::shared_ptr<RichMarkItem> item);

  std::vector<std::shared_ptr<RichMarkItem> > _markItems;

  std::array<float, 2> _latLon{};
  std::shared_ptr<RichMapPlot> _plot;
  std::unique_ptr<RichMarkItemWidget> _itemWidget;

  bool _isMousePick{false};
  std::string _markNameInputText{};
  bool _isMarkAdd{false};

  inline static const char _latLonFormat[]{"%.6f"};
};
} // namespace ImOsm