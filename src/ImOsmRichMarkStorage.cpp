#include "ImOsmRichMarkStorage.h"
#include "ImOsmRichMarkItem.h"

namespace ImOsm {
namespace Rich {
MarkStorage::MarkStorage() = default;

MarkStorage::~MarkStorage() = default;

std::shared_ptr<MarkItem> MarkStorage::findMark(const std::string &name) const {
  const auto it{std::find_if(_markItems.begin(),
                             _markItems.end(),
                             [name](const ItemNode &node) {
                               return node.ptr->text() == name;
                             })};
  if (it != _markItems.end()) {
    return it->ptr;
  }
  return nullptr;
}

GeoCoords MarkStorage::findMark(const std::string &name, bool &ok) const {
  const auto ptr{findMark(name)};
  if (ptr) {
    ok = true;
    return ptr->geoCoords();
  }
  ok = false;
  return {};
}

void MarkStorage::loadState(const mINI::INIStructure &ini) {
  for (auto it{ini.begin()}; it != ini.end(); ++it) {
    if (it->first.starts_with("mark_")) {
      auto ptr{std::make_shared<MarkItem>()};
      if (it->second.has("text")) {
        ptr->setText(it->second.get("text"));
      }
      if (it->second.has("lat") && it->second.has("lon")) {
        ptr->setCoords({std::stof(it->second.get("lat")),
                        std::stof(it->second.get("lon"))});
      }
      if (it->second.has("radius")) {
        ptr->setRadius(std::stof(it->second.get("radius")));
      }
      if (it->second.has("text_enabled")) {
        ptr->style().textEnabled = std::stoi(it->second.get("text_enabled"));
      }
      if (it->second.has("marker_enabled")) {
        ptr->style().markerEnabled =
            std::stoi(it->second.get("marker_enabled"));
      }
      if (it->second.has("radius_enabled")) {
        ptr->style().radiusEnabled =
            std::stoi(it->second.get("radius_enabled"));
      }
      if (it->second.has("radius_weight")) {
        ptr->style().radiusWeight = std::stof(it->second.get("radius_weight"));
      }
      if (it->second.has("marker_type")) {
        ptr->style().markerType = std::stoi(it->second.get("marker_type"));
      }
      if (it->second.has("marker_size")) {
        ptr->style().markerSize = std::stof(it->second.get("marker_size"));
      }
      if (it->second.has("marker_weight")) {
        ptr->style().markerWeight = std::stof(it->second.get("marker_weight"));
      }
      if (it->second.has("marker_fill_x") && it->second.has("marker_fill_y") &&
          it->second.has("marker_fill_z") && it->second.has("marker_fill_w")) {
        ptr->style().markerFill = {std::stof(it->second.get("marker_fill_x")),
                                   std::stof(it->second.get("marker_fill_y")),
                                   std::stof(it->second.get("marker_fill_z")),
                                   std::stof(it->second.get("marker_fill_w"))};
      }

      _markItems.push_back({ptr});
    }
  }
  _loadState = true;
}

void MarkStorage::saveState(mINI::INIStructure &ini) const {
  static const auto key_base{"mark_"};
  unsigned index{};
  for (const auto &item : _markItems) {
    const std::string key{key_base + std::to_string(index++)};
    ini[key]["text"] = item.ptr->text();
    ini[key]["lat"] = std::to_string(item.ptr->geoCoords().lat);
    ini[key]["lon"] = std::to_string(item.ptr->geoCoords().lon);
    ini[key]["radius"] = std::to_string(item.ptr->radius());
    ini[key]["text_enabled"] = std::to_string(item.ptr->style().textEnabled);
    ini[key]["marker_enabled"] =
        std::to_string(item.ptr->style().markerEnabled);
    ini[key]["radius_enabled"] =
        std::to_string(item.ptr->style().radiusEnabled);
    ini[key]["radius_weight"] = std::to_string(item.ptr->style().radiusWeight);
    ini[key]["marker_type"] = std::to_string(item.ptr->style().markerType);
    ini[key]["marker_size"] = std::to_string(item.ptr->style().markerSize);
    ini[key]["marker_weight"] = std::to_string(item.ptr->style().markerWeight);
    ini[key]["marker_fill_x"] = std::to_string(item.ptr->style().markerFill.x);
    ini[key]["marker_fill_y"] = std::to_string(item.ptr->style().markerFill.y);
    ini[key]["marker_fill_z"] = std::to_string(item.ptr->style().markerFill.z);
    ini[key]["marker_fill_w"] = std::to_string(item.ptr->style().markerFill.w);
  }
}

void MarkStorage::addMark(const GeoCoords &coords, const std::string &name) {
  _markItems.push_back({std::make_shared<MarkItem>(coords, name)});
}

void MarkStorage::rmMarks() {
  _markItems.erase(std::remove_if(_markItems.begin(), _markItems.end(),
                                  [](auto &item) { return item.rmFlag; }),
                   _markItems.end());
}

bool MarkStorage::handleLoadState() {
  const bool loadState{_loadState};
  _loadState = false;
  return loadState;
}

bool MarkStorage::handlePickState() {
  const bool pickState{_pickState};
  _pickState = false;
  return pickState;
}

} // namespace Rich
} // namespace ImOsm
