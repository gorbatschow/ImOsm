#pragma once
#include "ImOsmRichMarkItem.h"
#include <algorithm>
#include <array>
#include <memory>
#include <vector>

namespace ImOsm {
class RichMarkStorage {
public:
  RichMarkStorage() {}

  bool findMark(std::array<float, 2> &mark, const std::string &name) const {
    const auto it{std::find_if(
        _markItems.begin(), _markItems.end(),
        [name](const ItemNode &node) { return node.ptr->text() == name; })};
    if (it != _markItems.end()) {
      mark = {(*it).ptr->lat(), (*it).ptr->lon()};
      return true;
    }
    return false;
  }

private:
  friend class RichMarkWidget;
  struct ItemNode {
    std::shared_ptr<RichMarkItem> ptr;
    mutable bool rmFlag{false};
  };
  std::vector<ItemNode> _markItems;

  inline std::vector<ItemNode> &markItems() { return _markItems; }
  inline const std::vector<ItemNode> &markItems() const { return _markItems; }

  void addMark(const std::array<float, 2> &latlon, const std::string &name) {
    _markItems.push_back(
        {std::make_shared<RichMarkItem>(latlon[0], latlon[1], name)});
  }

  void rmMarks() {
    _markItems.erase(std::remove_if(_markItems.begin(), _markItems.end(),
                                    [](auto &item) { return item.rmFlag; }),
                     _markItems.end());
  }
};
} // namespace ImOsm
