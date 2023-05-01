#pragma once
#include "ImOsmCoords.h"
#include <ini.h>
#include <memory>
#include <vector>

namespace ImOsm {
namespace Rich {
class IRichItem;
class MarkItem;

class MarkStorage {
  friend class MarkEditorWidget;

public:
  MarkStorage();
  ~MarkStorage();

  std::shared_ptr<MarkItem> findMark(const std::string &name) const;
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
  bool handlePickState();
  bool handleLoadState();

  struct ItemNode;
  inline std::vector<ItemNode> &markItems() { return _markItems; }
  inline const std::vector<ItemNode> &markItems() const { return _markItems; }

private:
  // Load State
  bool _loadState{false};

  // Pick State
  GeoCoords _pickCoords{};
  bool _pickState{false};

  // Mark Items
  struct ItemNode {
    std::shared_ptr<MarkItem> ptr;
    mutable bool rmFlag{false};
  };
  std::vector<ItemNode> _markItems;
};
} // namespace Rich
} // namespace ImOsm
