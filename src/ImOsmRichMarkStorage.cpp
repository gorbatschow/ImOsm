#include "ImOsmRichMarkStorage.h"

bool ImOsm::RichMarkStorage::findMark(std::array<float, 2> &mark,
                                      const std::string &name) const {
  const auto it{std::find_if(_markItems.begin(),
                             _markItems.end(),
                             [name](const ItemNode &node) {
                               return node.ptr->text() == name;
                             })};
  if (it != _markItems.end()) {
    mark = {(*it).ptr->lat(), (*it).ptr->lon()};
    return true;
  }
  return false;
}

void ImOsm::RichMarkStorage::loadState(const mINI::INIStructure &ini) {
  for (auto it{ini.begin()}; it != ini.end(); ++it) {
    if (it->first.starts_with("mark_")) {
      auto ptr{std::make_shared<RichMarkItem>()};
      if (it->second.has("text")) {
        ptr->setText(it->second.get("text"));
      }
      if (it->second.has("lat") && it->second.has("lon")) {
        ptr->setLatLon(std::stof(it->second.get("lat")),
                       std::stof(it->second.get("lon")));
      }
      if (it->second.has("radius")) {
        ptr->setRadius(std::stof(it->second.get("radius")));
      }
      if (it->second.has("text_enabled")) {
        ptr->style().textEnabled = std::stoi(it->second.get("text_enabled"));
      }
      if (it->second.has("marker_enabled")) {
        ptr->style().markerEnabled = std::stoi(
            it->second.get("marker_enabled"));
      }
      if (it->second.has("radius_enabled")) {
        ptr->style().markerEnabled = std::stoi(
            it->second.get("radius_enabled"));
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
      if (it->second.has("marker_fill_x") && it->second.has("marker_fill_y")
          && it->second.has("marker_fill_z")
          && it->second.has("marker_fill_w")) {
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

void ImOsm::RichMarkStorage::saveState(mINI::INIStructure &ini) const {
  static const auto key_base{"mark_"};
  unsigned index{};
  for (const auto &item : _markItems) {
    const std::string key{key_base + std::to_string(index++)};
    ini[key]["text"] = item.ptr->text();
    ini[key]["lat"] = std::to_string(item.ptr->lat());
    ini[key]["lon"] = std::to_string(item.ptr->lon());
    ini[key]["radius"] = std::to_string(item.ptr->radius());
    ini[key]["text_enabled"] = std::to_string(item.ptr->style().textEnabled);
    ini[key]["marker_enabled"] = std::to_string(item.ptr->style().markerEnabled);
    ini[key]["radius_enabled"] = std::to_string(item.ptr->style().radiusEnabled);
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

void ImOsm::RichMarkStorage::addMark(const std::array<float, 2> &latlon,
                                     const std::string &name) {
  _markItems.push_back(
      {std::make_shared<RichMarkItem>(latlon[0], latlon[1], name)});
}

void ImOsm::RichMarkStorage::rmMarks() {
  _markItems.erase(std::remove_if(_markItems.begin(),
                                  _markItems.end(),
                                  [](auto &item) { return item.rmFlag; }),
                   _markItems.end());
}
