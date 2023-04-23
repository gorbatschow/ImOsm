#pragma once
#include "ImOsmRichMapPlot.h"
#include "ImOsmRichMarkItemWidget.h"
#include "ImOsmRichMarkStorage.h"
#include <array>
#include <imgui.h>
#include <ini.h>

namespace ImOsm {
class RichMarkWidget {
public:
  RichMarkWidget(std::shared_ptr<RichMapPlot> plot,
                 std::shared_ptr<RichMarkStorage> storage);
  virtual ~RichMarkWidget() = default;

  void loadState(const mINI::INIStructure &ini);
  void saveState(mINI::INIStructure &ini) const;
  void paint();

private:
  void paint_latLonInput();
  void paint_mousePickBtn();
  void paint_markNameInput();
  void paint_addMarkBtn();
  void paint_markTable();
  void paint_markTableRow(const RichMarkStorage::ItemNode &item);

  std::array<float, 2> _latLonInput{0.f, 0.f};
  std::shared_ptr<RichMapPlot> _plot;
  std::shared_ptr<RichMarkStorage> _storage;
  std::unique_ptr<RichMarkItemWidget> _itemWidget;

  bool _isMousePick{false};
  std::string _markNameInputText{};
  bool _isMarkAdd{false};

  inline static const char _latlonFormat[]{"%.6f"};
};
} // namespace ImOsm
