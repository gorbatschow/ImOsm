#pragma once
#include "ImOsmTileLoader.h"
#include "ImOsmTileSourceFs.h"
#include "ImOsmTileSourceUrlImpl.h"
#include <memory>

namespace ImOsm {

inline constexpr int URL_REQUEST_LIMIT{10};
inline constexpr int FS_REQUEST_LIMIT{10};
inline bool MAP_PRELOAD{true};

class TileLoaderOsmMap : public TileLoader {
public:
  TileLoaderOsmMap(const int requestLimit = URL_REQUEST_LIMIT)
      : TileLoader{
            std::make_shared<TileSourceUrlOsm>(requestLimit, MAP_PRELOAD)} {}
};

class TileLoaderArcMap : public TileLoader {
public:
  TileLoaderArcMap(const int requestLimit = URL_REQUEST_LIMIT)
      : TileLoader{
            std::make_shared<TileSourceUrlArc>(requestLimit, MAP_PRELOAD)} {}
};

class TileLoaderUrlMap : public TileLoader {
public:
  TileLoaderUrlMap(const std::string &url_tpl,
                   const int requestLimit = URL_REQUEST_LIMIT)
      : TileLoader{std::make_shared<TileSourceUrlCustom>(
            requestLimit, MAP_PRELOAD, url_tpl)} {}
};

class TileLoaderFsMap : public TileLoader {
public:
  TileLoaderFsMap(const std::string &dirname,
                  const int requestLimit = URL_REQUEST_LIMIT)
      : TileLoader{std::make_shared<TileSourceFsSubDir>(requestLimit,
                                                        MAP_PRELOAD, dirname)} {
  }
};

} // namespace ImOsm
