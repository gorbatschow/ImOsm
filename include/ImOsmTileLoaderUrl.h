#pragma once
#include "ImOsmITileLoader.h"
#include "ImOsmTileTexture.h"
#include <array>
#include <chrono>
#include <curl/curl.h>
#include <future>
#include <list>
#include <map>
#include <memory>

namespace ImOsm {
namespace Old {
class TileLoaderUrl : public ITileLoader {
public:
  TileLoaderUrl();
  ~TileLoaderUrl();

  void beginLoad(int z, int xmin, int xmax, int ymin, int ymax) override;
  ImTextureID tileAt(int z, int x, int y) override;
  int tileCount() const override { return _tiles.size(); }

  inline void setTileProvider(const std::string &url) { _tileProvider = url; }
  inline const std::string &tileProvider() const { return _tileProvider; }

  inline void setTileExtension(const std::string &ext) { _tileExtension = ext; }
  inline const std::string &tileExtensioin() const { return _tileExtension; }

  inline void setClientName(const std::string &name) { _clientName = name; }
  inline const std::string &clientName() const { return _clientName; }

private:
  struct Tile {
    struct Remote {
      std::vector<std::byte> blob;
      std::shared_ptr<TileTexture> texture;
      CURLcode code{CURLE_OK};
    };
    std::array<int, 3> zxy{0, 0, 0};
    std::future<Remote> future;
    std::shared_ptr<TileTexture> texture;
    bool operator==(const Tile &other) { return this->zxy == other.zxy; }
  };

  std::string _clientName{"curl"};
  std::string _tileProvider{"https://server.arcgisonline.com/ArcGIS/rest/"
                            "services/World_Imagery/MapServer/tile/"};
  std::string _tileExtension{".png"};

  const int _tileSizePx{256};
  TileTexture _blankTile{_tileSizePx, TextureColor::Slate};
  std::list<Tile> _tiles;
  const int _futureLimit{4}, _textureLimit{1};
  int _textureCounter{0}, _futureCounter{0};

  Tile::Remote onHandleRequest(const std::array<int, 3> &zxy);

  static size_t onPullResponse(void *data, size_t size, size_t nmemb,
                               void *userp);
};
} // namespace Old
} // namespace ImOsm
