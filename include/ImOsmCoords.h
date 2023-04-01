#pragma once
#include <cmath>

namespace ImOsm {
template<typename T>
inline T lon2x(T lon, int z) {
  return ((lon + 180.0) / 360.0 * T(1 << z));
}

template<typename T>
inline T lat2y(T lat, int z) {
  lat = lat * M_PI / 180.0;
  return (1.0 - asinh(tan(lat)) / M_PI) / 2.0 * T(1 << z);
}

template<typename T>
inline T x2lon(T x, int z) {
  return x / T(1 << z) * 360.0 - 180.0;
}

template<typename T>
inline T y2lat(T y, int z) {
  T n = M_PI - 2.0 * M_PI * y / T(1 << z);
  return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

template<typename T>
inline int lon2tx(T lon, int z) {
  return int(floor(lon2x(lon, z)));
}

template<typename T>
inline int lat2ty(T lat, int z) {
  return int(floor(lat2y(lat, z)));
}

constexpr static const float MinLat{-85.0f};
constexpr static const float MaxLat{+85.0f};
constexpr static const float MinLon{-179.9f};
constexpr static const float MaxLon{+179.9f};
constexpr static const int MinZoom{0};
constexpr static const int MaxZoom{18};

} // namespace ImOsm
