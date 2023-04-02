#pragma once

namespace ImOsm {
class IRichItem
{
public:
  virtual ~IRichItem() = default;

  virtual bool inBounds(float minLat,
                        float maxLat,
                        float minLon,
                        float maxLon) const
      = 0;
  virtual void setEnabled(bool enabled) = 0;
  virtual bool enabled() const = 0;
  virtual void paint() = 0;
};
} // namespace ImOsm
