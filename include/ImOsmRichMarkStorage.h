#pragma once
#include "ImOsmRichMarkItem.h"
#include <algorithm>
#include <array>
#include <ini.h>
#include <memory>
#include <vector>

namespace ImOsm {
class RichMarkStorage
{
  friend class RichMarkWidget;

public:
  RichMarkStorage() = default;
  ~RichMarkStorage() = default;

  bool findMark(std::array<float, 2> &mark, const std::string &name) const;
  void loadState(const mINI::INIStructure &ini);
  void saveState(mINI::INIStructure &ini) const;

private:
  void addMark(const std::array<float, 2> &latlon, const std::string &name);
  void rmMarks();

private:
  bool _loadState{false};
  inline bool handleLoadState() {
    const bool loadState{_loadState};
    _loadState = false;
    return loadState;
  }

  struct ItemNode {
    std::shared_ptr<RichMarkItem> ptr;
    mutable bool rmFlag{false};
  };
  std::vector<ItemNode> _markItems;
  inline std::vector<ItemNode> &markItems() { return _markItems; }
  inline const std::vector<ItemNode> &markItems() const { return _markItems; }
};
} // namespace ImOsm
