#pragma once
#include "ImOsmTileTexture.h"
#include <array>
#include <chrono>
#include <curl/curl.h>
#include <future>
#include <list>
#include <map>
#include <memory>

class OsmTileLoader {
public:
  OsmTileLoader();
  ~OsmTileLoader();

  void beginLoad(int z, int xmin, int xmax, int ymin, int ymax);
  ImTextureID tileAt(int z, int x, int y);
  inline int tilesNum() const { return _tiles.size(); }

  inline void setTileProvider(const std::string &url) { _tileProvider = url; }
  inline const std::string &tileProvider() const { return _tileProvider; }

  inline void setTileExtension(const std::string &ext) { _tileExtension = ext; }
  inline const std::string &tileExtensioin() const { return _tileExtension; }

  inline void setClientName(const std::string &name) { _clientName = name; }
  inline const std::string &clientName() const { return _clientName; }

private:
  struct RemoteTile {
    std::vector<std::byte> blob;
    std::shared_ptr<OsmTileTexture> texture;
    CURLcode code{CURLE_OK};
  };

  struct Tile {
    std::array<int, 3> zxy{0, 0, 0};
    std::future<RemoteTile> future;
    std::shared_ptr<OsmTileTexture> texture;
    bool operator==(const Tile &other) { return this->zxy == other.zxy; }
  };

  std::string _clientName{"curl"};
  std::string _tileProvider{"http://a.tile.openstreetmap.org/"};
  std::string _tileExtension{".png"};

  const int _tileSizePx{256};

  OsmTileTexture _blankTile{_tileSizePx, TextureColor::Slate};
  std::list<Tile> _tiles;

  const int _futureLimit{4}, _textureLimit{1};
  int _textureCounter{0}, _futureCounter{0};

  RemoteTile onHandleRequest(const std::array<int, 3> &zxy);

  static size_t onPullResponse(void *data, size_t size, size_t nmemb,
                               void *userp);
};
