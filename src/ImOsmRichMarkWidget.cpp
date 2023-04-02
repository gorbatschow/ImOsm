#include "ImOsmRichMarkWidget.h"
#include "ImOsmRichMarkItemWidget.h"
#include <misc/cpp/imgui_stdlib.h>

ImOsm::RichMarkWidget::RichMarkWidget(std::shared_ptr<RichMapPlot> plot)
    : _plot{plot} {
  _markNameInputText.reserve(32);
}

void ImOsm::RichMarkWidget::paint() {
  paint_latLonInput();
  ImGui::SameLine();
  paint_mousePickBtn();
  paint_markNameInput();
  ImGui::SameLine();
  paint_addMarkBtn();
  paint_markTable();

  if (_isMousePick && _plot->mouseOnPlot() && ImGui::IsMouseClicked(0)) {
    _isMousePick = false;
    _latLon[0] = _plot->mouseLat();
    _latLon[1] = _plot->mouseLon();
  }

  if (_isMarkAdd) {
    _isMarkAdd = false;
    _markItems.push_back(std::make_shared<RichMarkItem>(_latLon[0],
                                                        _latLon[1],
                                                        _markNameInputText));
    _plot->addItem(_markItems.back());
  }
}

void ImOsm::RichMarkWidget::paint_latLonInput() {
  ImGui::InputFloat2("Lat/Lon##", _latLon.data(), _latLonFormat);
}

void ImOsm::RichMarkWidget::paint_mousePickBtn() {
  if (!_isMousePick) {
    if (ImGui::Button("Mouse Pick##")) {
      _isMousePick = !_isMousePick;
    }
  } else {
    const auto color{ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered)};
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    if (ImGui::Button("Mouse Pick")) {
      _isMousePick = !_isMousePick;
    }
    ImGui::PopStyleColor();
  }
}

void ImOsm::RichMarkWidget::paint_markNameInput() {
  ImGui::InputText("Mark Name##", &_markNameInputText);
}

void ImOsm::RichMarkWidget::paint_addMarkBtn() {
  if (ImGui::Button("Add Mark##")) {
    _isMarkAdd = true;
  }
}

void ImOsm::RichMarkWidget::paint_markTable() {
  static const int tableCols{4};
  static const ImGuiTableColumnFlags colFlags{ImGuiTableColumnFlags_WidthFixed};

  if (ImGui::BeginTable("MarkTabe", 5)) {
    ImGui::TableSetupColumn("Name", colFlags, 200);
    ImGui::TableSetupColumn("Lat", colFlags, 100);
    ImGui::TableSetupColumn("Lon", colFlags, 100);
    ImGui::TableSetupColumn("Setup", colFlags, 50);
    ImGui::TableSetupColumn("Delete", colFlags, 50);
    ImGui::TableHeadersRow();
    std::for_each(_markItems.begin(), _markItems.end(), [this](auto item) {
      ImGui::TableNextRow();
      ImGui::PushID(item.get());
      paint_markTableRow(item);
      ImGui::PopID();
    });
    ImGui::EndTable();
  }
}

void ImOsm::RichMarkWidget::paint_markTableRow(
    std::shared_ptr<RichMarkItem> item) {
  // Name
  ImGui::TableNextColumn();
  ImGui::TextUnformatted(item->text().c_str());

  // Lat
  ImGui::TableNextColumn();
  ImGui::Text(_latLonFormat, item->lat());

  // Lon
  ImGui::TableNextColumn();
  ImGui::Text(_latLonFormat, item->lon());

  // Setup
  ImGui::TableNextColumn();
  if (ImGui::Button("Setup##")) {
    ImGui::OpenPopup("Setup Item##");
    _itemWidget = std::make_unique<RichMarkItemWidget>(item, _latLon);
  }

  if (ImGui::BeginPopupModal("Setup Item##")) {
    // Draw popup contents.
    ImOsm::RichMarkItemWidget w(item);
    _itemWidget->paint();
    if (ImGui::Button("Cancel##")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Apply##")) {
      _itemWidget->apply();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  // Delete
  ImGui::TableNextColumn();
  if (ImGui::Button("Delete##")) {
  }
}
