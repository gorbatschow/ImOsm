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
inline constexpr int POW2[]{
    (1 << 0),  (1 << 1),  (1 << 2),  (1 << 3),  (1 << 4),  (1 << 5),  (1 << 6),
    (1 << 7),  (1 << 8),  (1 << 9),  (1 << 10), (1 << 11), (1 << 12), (1 << 13),
    (1 << 14), (1 << 15), (1 << 16), (1 << 17), (1 << 18)};

inline constexpr double MIN_LAT{-85.0};
inline constexpr double MAX_LAT{+85.0};
inline constexpr double MIN_LON{-179.9};
inline constexpr double MAX_LON{+179.9};
inline constexpr int MIN_ZOOM{0};
inline constexpr int MAX_ZOOM{18};

inline double lon2x(const double lon, int z = 0) {
  return (lon + 180.0) / 360.0 * double(POW2[z]);
}

inline double lat2y(const double lat, int z = 0) {
  return (1.0 - asinh(tan(lat * RAD)) / PI) / 2.0 * double(POW2[z]);
}

inline double x2lon(const double x, int z = 0) {
  return x / double(POW2[z]) * 360.0 - 180.0;
}

inline double y2lat(const double y, const int z = 0) {
  const double n{PI - PI2 * y / double(POW2[z])};
  return DEG * atan(0.5 * (exp(n) - exp(-n)));
}

inline int lon2tx(const double lon, const int z) {
  return int(floor(lon2x(lon, z)));
}

inline int lat2ty(const double lat, const int z) {
  return int(floor(lat2y(lat, z)));
}

inline std::pair<int, int> minmax_tx(double minLon, double maxLon, int z) {

  minLon = std::clamp(minLon, MIN_LON, MAX_LON);
  maxLon = std::clamp(maxLon, MIN_LON, MAX_LON);
  z = std::clamp(z, MIN_ZOOM, MAX_ZOOM);
  std::pair<int, int> mm{std::minmax(lon2tx(minLon, z), lon2tx(maxLon, z))};
  mm.second = std::clamp(mm.second, 0, POW2[z] - 1);
  return mm;
}

inline std::pair<int, int> minmax_ty(double minLat, double maxLat, int z) {

  minLat = std::clamp(minLat, MIN_LAT, MAX_LAT);
  maxLat = std::clamp(maxLat, MIN_LAT, MAX_LAT);
  z = std::clamp(z, MIN_ZOOM, MAX_ZOOM);
  std::pair<int, int> mm{std::minmax(lat2ty(minLat, z), lat2ty(maxLat, z))};
  mm.second = std::clamp(mm.second, 0, POW2[z] - 1);
  return mm;
}

inline size_t countTiles(double minLat, double maxLat, double minLon,
                         double maxLon, int minZ, int maxZ) {

  if (minZ > maxZ || minLat > maxLat || minLon > maxLon) {
    return 0;
  }

  size_t counter{};
  for (auto z{minZ}; z != maxZ + 1; ++z) {
    auto tx{minmax_tx(minLon, maxLon, z)};
    auto ty{minmax_ty(minLat, maxLat, z)};
    size_t total{size_t(tx.second - tx.first + 1) *
                 size_t(ty.second - ty.first + 1)};
    counter += total;
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
  GeoCoords() = default;
  GeoCoords(double lat_, double lon_) : lat{lat_}, lon{lon_} {}
  GeoCoords(const std::array<double, 2> &arr)
      : lat{arr.front()}, lon{arr.back()} {}
  GeoCoords(const std::array<float, 2> &arr)
      : lat{arr.front()}, lon{arr.back()} {}

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
