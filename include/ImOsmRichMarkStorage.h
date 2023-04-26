#pragma once
#include "ImOsmCoords.h"
#include "ImOsmRichMarkItem.h"
#include <algorithm>
#include <ini.h>
#include <memory>
#include <vector>

namespace ImOsm {
namespace Rich {
class MarkStorage {
  friend class MarkEditorWidget;

public:
  MarkStorage() = default;
  ~MarkStorage() = default;

  GeoCoords findMark(const std::string &name, bool &ok) const;
  void loadState(const mINI::INIStructure &ini);
  void saveState(mINI::INIStructure &ini) const;

  inline void setPickCoords(const GeoCoords &coords) {
    _pickCoords = coords;
    _pickState = true;
  }

private:
  void addMark(const GeoCoords &coords, const std::string &name);
  void rmMarks();

private:
  // Load State
  bool _loadState{false};
  inline bool handleLoadState() {
    const bool loadState{_loadState};
    _loadState = false;
    return loadState;
  }

  // Pick State
  GeoCoords _pickCoords{};
  bool _pickState{false};
  inline bool handlePickState() {
    const bool pickState{_pickState};
    _pickState = false;
    return pickState;
  }

  // Mark Items
  struct ItemNode {
    std::shared_ptr<MarkItem> ptr;
    mutable bool rmFlag{false};
  };
  std::vector<ItemNode> _markItems;
  inline std::vector<ItemNode> &markItems() { return _markItems; }
  inline const std::vector<ItemNode> &markItems() const { return _markItems; }
};
} // namespace Rich
} // namespace ImOsm
