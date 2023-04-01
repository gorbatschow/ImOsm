#pragma once
#include "ImOsmIRichItem.h"
#include "ImOsmMapPlot.h"
#include <algorithm>
#include <vector>

namespace ImOsm {
enum class GeoMarkType { Text };

class RichMapPlot : public MapPlot
{
public:
  RichMapPlot() {}
  virtual ~RichMapPlot() = default;

  void addGeoItem(std::weak_ptr<IRichItem> item) { _items.push_back(item); }

  virtual void paintOverMap() override {
    MapPlot::paintOverMap();
    /*
    _items.erase(
        std::remove_if(_items.begin(), _items.end(), [](auto item) { return item.expired(); }));
*/

    std::for_each(_items.begin(), _items.end(), [this](auto ptr) {
      auto item{ptr.lock()};
      if (item->inBounds(minLat(), maxLat(), minLon(), maxLon()))
        item->paint();
    });
  }

private:
  std::vector<std::weak_ptr<IRichItem> > _items;
};
} // namespace ImOsm
