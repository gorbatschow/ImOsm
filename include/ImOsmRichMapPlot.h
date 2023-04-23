#pragma once
#include "ImOsmIRichItem.h"
#include "ImOsmMapPlot.h"
#include <algorithm>
#include <imgui.h>
#include <iostream>
#include <vector>

namespace ImOsm {

namespace Rich {
enum class GeoMarkType { Text };

class RichMapPlot : public ImOsm::MapPlot
{
public:
  RichMapPlot() {}
  virtual ~RichMapPlot() = default;

  virtual void paintOverMap() override {
    ImOsm::MapPlot::paintOverMap();

    _items.erase(std::remove_if(_items.begin(),
                                _items.end(),
                                [](auto item) { return item.expired(); }),
                 _items.end());

    std::for_each(_items.begin(), _items.end(), [this](auto ptr) {
      auto item{ptr.lock()};
      if (item->enabled()
          && item->inBounds(minLat(), maxLat(), minLon(), maxLon()))
        item->paint();
    });
  }

  inline void addItem(std::weak_ptr<IRichItem> item) { _items.push_back(item); }

private:
  std::vector<std::weak_ptr<IRichItem> > _items;
};
} // namespace Rich
} // namespace ImOsm
