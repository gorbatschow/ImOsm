#include "ImOsmRichMarkWidget.h"
#include "ImOsmRichMarkItemWidget.h"
#include <memory>
#include <misc/cpp/imgui_stdlib.h>
ImOsm::RichMarkWidget::RichMarkWidget(std::shared_ptr<RichMapPlot> plot,
                                      std::shared_ptr<RichMarkStorage> storage)
    : _plot{plot}, _storage{storage} {
  _markNameInputText.reserve(32);
}

void ImOsm::RichMarkWidget::loadState(const mINI::INIStructure &ini) {}

void ImOsm::RichMarkWidget::saveState(mINI::INIStructure &ini) const {}

void ImOsm::RichMarkWidget::paint() {
  ImGui::TextUnformatted("Mark Editor");
  paint_latLonInput();
  ImGui::SameLine();
  paint_mousePickBtn();
  paint_markNameInput();
  ImGui::SameLine();
  paint_addMarkBtn();
  paint_markTable();

  if (_isMousePick && _plot->mouseOnPlot() && ImGui::IsMouseClicked(0)) {
    _isMousePick = false;
    _latLonInput = {_plot->mouseLat(), _plot->mouseLon()};
  }

  if (_isMarkAdd) {
    _isMarkAdd = false;
    _storage->addMark(_latLonInput, _markNameInputText);
    _plot->addItem(_storage->_markItems.back().ptr);
  }

  if (_storage->handleLoadState()) {
    const auto &markItems{_storage->markItems()};
    std::for_each(markItems.begin(), markItems.end(),
                  [this](auto &item) { _plot->addItem(item.ptr); });
  }

  if (_storage->handlePickState()) {
    _latLonInput = _storage->_pickCoords;
  }
}

void ImOsm::RichMarkWidget::paint_latLonInput() {
  ImGui::InputFloat2("Lat/Lon", _latLonInput.data(), _latlonFormat);
}

void ImOsm::RichMarkWidget::paint_mousePickBtn() {
  if (!_isMousePick) {
    if (ImGui::Button("Mouse Pick")) {
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
  ImGui::InputText("Mark Name", &_markNameInputText);
}

void ImOsm::RichMarkWidget::paint_addMarkBtn() {
  if (ImGui::Button("Add Mark")) {
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

    _storage->rmMarks();

    const auto &markItems{_storage->markItems()};
    std::for_each(markItems.begin(), markItems.end(), [this](auto &item) {
      ImGui::TableNextRow();
      ImGui::PushID(item.ptr.get());
      paint_markTableRow(item);
      ImGui::PopID();
    });

    ImGui::EndTable();
  }
}

void ImOsm::RichMarkWidget::paint_markTableRow(
    const RichMarkStorage::ItemNode &item) {
  // Name
  ImGui::TableNextColumn();
  ImGui::TextUnformatted(item.ptr->text().c_str());

  // Lat
  ImGui::TableNextColumn();
  ImGui::Text(_latlonFormat, item.ptr->geoCoords().lat);

  // Lon
  ImGui::TableNextColumn();
  ImGui::Text(_latlonFormat, item.ptr->geoCoords().lon);

  // Setup
  ImGui::TableNextColumn();
  if (ImGui::Button("Setup")) {
    ImGui::OpenPopup("Setup Item");
    _itemWidget = std::make_unique<RichMarkItemWidget>(item.ptr,
                                                       GeoCoords{_latLonInput});
  }

  if (ImGui::BeginPopupModal("Setup Item")) {
    // Draw popup contents.
    _itemWidget->paint();
    ImGui::Separator();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      _itemWidget.reset();
    }
    ImGui::SameLine();
    if (ImGui::Button("Apply")) {
      _itemWidget->apply();
      ImGui::CloseCurrentPopup();
      _itemWidget.reset();
    }
    ImGui::EndPopup();
  }

  // Delete
  ImGui::TableNextColumn();
  if (ImGui::Button("Delete")) {
    item.rmFlag = true;
  }
}
