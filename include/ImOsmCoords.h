#pragma once
#include <array>
#include <math.h>
#include <numbers>

namespace ImOsm {

inline constexpr double PI{std::numbers::pi_v<double>};
inline constexpr double PI2{std::numbers::pi_v<double> * 2.0};
inline constexpr double RAD{std::numbers::pi_v<double> / 180.0};
inline constexpr double DEG{180.0 / std::numbers::pi_v<double>};
inline constexpr double R{6371.0};

inline double lon2x(double lon, int z) {
  return ((lon + 180.0) / 360.0 * (1 << z));
}

inline double lat2y(double lat, int z) {
  lat *= RAD;
  return (1.0 - asinh(tan(lat)) / PI) / 2.0 * double(1 << z);
}

inline double x2lon(double x, int z) { return x / (1 << z) * 360.0 - 180.0; }

inline double y2lat(double y, int z) {
  const double n{PI - PI2 * y / double(1 << z)};
  return DEG * atan(0.5 * (exp(n) - exp(-n)));
}

inline int lon2tx(double lon, int z) { return int(floor(lon2x(lon, z))); }

inline int lat2ty(double lat, int z) { return int(floor(lat2y(lat, z))); }

inline constexpr double MinLat{-85.0};
inline constexpr double MaxLat{+85.0};
inline constexpr double MinLon{-179.9};
inline constexpr double MaxLon{+179.9};
inline constexpr int MinZoom{0};
inline constexpr int MaxZoom{18};

inline std::array<double, 2> latlon(const std::array<double, 2> &latlon,
                                    double d, double tc = 0.0) {
  d /= R;
  tc *= RAD;
  const double lat1{latlon[0] * RAD};
  const double lon1{latlon[1] * RAD};
  const double lat2{asin(sin(lat1) * cos(d) + cos(lat1) * sin(d) * cos(tc))};
  const double dlon{
      atan2(sin(tc) * sin(d) * cos(lat1), cos(d) - sin(lat1) * sin(lat2))};
  const double lon2{fmod(lon1 - dlon + PI, PI2) - PI};
  return {lat2 * DEG, lon2 * DEG};
}

inline double distance(const std::array<double, 2> &src,
                       const std::array<double, 2> &dst) {
  const double lat1{src[0] * RAD};
  const double lon1{src[1] * RAD};
  const double lat2{dst[0] * RAD};
  const double lon2{dst[1] * RAD};
  const double dlat{lat2 - lat1};
  const double dlon{lon2 - lon1};
  const double a{sin(dlat / 2.0) * sin(dlat / 2.0) +
                 cos(lat1) * cos(lat2) * sin(dlon / 2.0) * sin(dlon / 2.0)};
  return 2.0 * atan2(sqrt(a), sqrt(1.0 - a)) * R;
}

} // namespace ImOsm
