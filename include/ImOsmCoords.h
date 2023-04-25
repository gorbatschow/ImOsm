#pragma once
#include "implot.h"
#include <algorithm>
#include <array>
#include <latlon.h>
#include <math.h>
#include <numbers>

namespace ImOsm {

inline constexpr double PI{std::numbers::pi_v<double>};
inline constexpr double PI2{std::numbers::pi_v<double> * 2.0};
inline constexpr double RAD{std::numbers::pi_v<double> / 180.0};
inline constexpr double DEG{180.0 / std::numbers::pi_v<double>};
inline constexpr double R{6371.0};

inline constexpr double MinLat{-85.0};
inline constexpr double MaxLat{+85.0};
inline constexpr double MinLon{-179.9};
inline constexpr double MaxLon{+179.9};
inline constexpr int MinZoom{0};
inline constexpr int MaxZoom{18};

inline constexpr int POW2[]{1,     2,     4,     8,      16,    32,   64,
                            128,   256,   512,   102,    2048,  4096, 8192,
                            16384, 32768, 65536, 131072, 262144};

inline double lon2x(double lon, int z = 0) {
  return (lon + 180.0) / 360.0 * double(POW2[z]);
}

inline double lat2y(double lat, int z = 0) {
  lat *= RAD;
  return (1.0 - asinh(tan(lat)) / PI) / 2.0 * double(POW2[z]);
}

inline double x2lon(double x, int z = 0) {
  return x / double(POW2[z]) * 360.0 - 180.0;
}

inline double y2lat(double y, int z = 0) {
  const double n{PI - PI2 * y / double(POW2[z])};
  return DEG * atan(0.5 * (exp(n) - exp(-n)));
}

inline int lon2tx(double lon, int z) {
  return std::clamp(int(floor(lon2x(lon, z))), 0, POW2[z]);
}

inline int lat2ty(double lat, int z) {
  return std::clamp(int(floor(lat2y(lat, z))), 0, POW2[z]);
}

inline std::pair<int, int> minmax_lon2tx(const double minLon,
                                         const double maxLon, const int z) {
  std::pair<int, int> mm{std::minmax(lon2tx(minLon, z), lon2tx(maxLon, z))};
  mm.second += 1;
  return mm;
}

inline std::pair<int, int> minmax_lat2ty(const double minLat,
                                         const double maxLat, const int z) {
  std::pair<int, int> mm{std::minmax(lat2ty(minLat, z), lat2ty(maxLat, z))};
  mm.second += 1;
  return mm;
}

inline int countTiles(const double minLat, const double maxLat,
                      const double minLon, const double maxLon, const int minZ,
                      int maxZ) {
  int counter{};
  maxZ = std::clamp(maxZ, MinZoom, MaxZoom - 1);
  for (auto z{minZ}; z != maxZ + 1; ++z) {
    const auto tx{minmax_lon2tx(minLon, maxLon, z)};
    const auto ty{minmax_lat2ty(minLat, maxLat, z)};
    counter += (tx.second - tx.first) * (ty.second - ty.first);
  }
  return counter;
}

struct OsmCoords;
struct GeoCoords;
struct LocalCoords;

struct LocalCoords {
  double x{};
  double y{};
};

struct OsmCoords {
  double x{};
  double y{};

  inline ImPlotPoint toPlotPoint() const { return {x, y}; }
  inline GeoCoords toGeoCoords() const;
};

struct GeoCoords {
  double lat{};
  double lon{};

  inline ImPlotPoint toPlotPoint() const { return {lon2x(lon), lat2y(lat)}; }
  inline OsmCoords toOsmCoords() const { return {lon2x(lon), lat2y(lat)}; }
  inline double toOsmX() const { return lon2x(lon); };
  inline double toOsmY() const { return lat2y(lat); };
  inline LocalCoords toLocalCoords(const GeoCoords &o) const {
    LocalCoords c;
    LatLon::cartesian(c.x, c.y, lat, lon, o.lat, o.lon);
    return c;
  }
  inline GeoCoords destination(const double d, const double b) const {
    GeoCoords c;
    LatLon::destination(c.lat, c.lon, lat, lon, d, b);
    return c;
  }

  GeoCoords() = default;
  GeoCoords(double lat_, double lon_) : lat{lat_}, lon{lon_} {}
  GeoCoords(const std::array<double, 2> &arr)
      : lat{arr.front()}, lon{arr.back()} {}
  GeoCoords(const std::array<float, 2> &arr)
      : lat{arr.front()}, lon{arr.back()} {}

  inline double distance(const GeoCoords &other) const {
    return LatLon::distance(lat, lon, other.lat, other.lon);
  }

  inline double bearing(const GeoCoords &other) const {
    return LatLon::bearing(lat, lon, other.lat, other.lon);
  }

  inline GeoCoords midpoint(const GeoCoords &other) {
    double lat_{}, lon_{};
    LatLon::midpoint(lat_, lon_, lat, lon, other.lat, other.lon);
    return {lat_, lon_};
  }

  inline operator std::array<double, 2>() const { return {lat, lon}; }
  inline operator std::array<float, 2>() const {
    return {float(lat), float(lon)};
  }
};

inline GeoCoords OsmCoords::toGeoCoords() const { return {y2lat(y), x2lon(x)}; }

inline GeoCoords destination(const GeoCoords &src, double d, double b = 0.0) {
  GeoCoords c;
  LatLon::destination(c.lat, c.lon, src.lat, src.lon, d, b);
  return c;
}

inline double distance(const GeoCoords &a, const GeoCoords &b) {
  return LatLon::distance(a.lat, a.lon, b.lat, b.lon);
}

inline const GeoCoords midpoint(const GeoCoords &a, const GeoCoords &b) {
  GeoCoords c;
  LatLon::midpoint(c.lat, c.lon, a.lat, a.lon, b.lat, b.lon);
  return c;
}

} // namespace ImOsm
