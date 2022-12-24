#pragma once
#include "OsmTileTexture.h"
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

  template <typename T> struct Countable {
    Countable() { _alive++; }
    Countable(const Countable &) { _alive++; }
    static int alive() { return _alive; }

  protected:
    static int _alive;
    virtual ~Countable() { --_alive; }
  };

  struct RemoteTile : Countable<RemoteTile> {
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

private:
  const int _tileSizePx{256};
  OsmTileTexture _blankTile{_tileSizePx, TextureColor::Slate};
  std::list<Tile> _tiles;
  const int _futureLimit{4}, _textureLimit{1};
  int _textureCounter{0}, _futureCounter{0};

  RemoteTile onHandleRequest(const std::array<int, 3> &zxy);

  static size_t onPullResponse(void *data, size_t size, size_t nmemb,
                               void *userp);
};

template <typename T> int OsmTileLoader::Countable<T>::_alive(0);
