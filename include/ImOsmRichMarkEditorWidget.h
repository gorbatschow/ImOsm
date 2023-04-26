#pragma once
#include "ImOsmRichMarkStorage.h"
#include <ini.h>

namespace ImOsm {
namespace Rich {
class RichMapPlot;
class MarkStorage;
class MarkItemWidget;

class MarkEditorWidget {
public:
  MarkEditorWidget(std::shared_ptr<RichMapPlot> plot,
                   std::shared_ptr<MarkStorage> storage);
  ~MarkEditorWidget();

  void paint();

private:
  void paint_latLonInput();
  void paint_mousePickBtn();
  void paint_markNameInput();
  void paint_addMarkBtn();
  void paint_markTable();
  void paint_markTableRow(const MarkStorage::ItemNode &item);

  std::shared_ptr<RichMapPlot> _plot;
  std::shared_ptr<MarkStorage> _storage;
  std::unique_ptr<MarkItemWidget> _itemWidget;

  struct Ui;
  std::unique_ptr<Ui> _ui;
};
} // namespace Rich
} // namespace ImOsm
