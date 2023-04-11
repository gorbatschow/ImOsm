#pragma once
#include "ImOsmRichMapPlot.h"
#include "ImOsmRichMarkItemWidget.h"
#include "ImOsmRichMarkStorage.h"
#include <array>
#include <imgui.h>

namespace ImOsm {
class RichMarkWidget {
public:
  RichMarkWidget(std::shared_ptr<RichMapPlot> plot,
                 std::shared_ptr<RichMarkStorage> storage);
  virtual ~RichMarkWidget() = default;

  void paint();

private:
  void paint_latLonInput();
  void paint_mousePickBtn();
  void paint_markNameInput();
  void paint_addMarkBtn();
  void paint_markTable();
  void paint_markTableRow(const RichMarkStorage::ItemNode &item);

  std::array<float, 2> _latlon{};
  std::shared_ptr<RichMapPlot> _plot;
  std::shared_ptr<RichMarkStorage> _storage;
  std::unique_ptr<RichMarkItemWidget> _itemWidget;

  bool _isMousePick{false};
  std::string _markNameInputText{};
  bool _isMarkAdd{false};

  inline static const char _latlonFormat[]{"%.6f"};
};
} // namespace ImOsm
