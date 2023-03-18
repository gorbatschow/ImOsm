#include "ImOsmTileLoader.h"
#include <GL/gl.h>
#include <algorithm>
#include <chrono>
#include <curl/curl.h>
#include <future>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>
using namespace std::chrono_literals;

OsmTileLoader::OsmTileLoader() {}

OsmTileLoader::~OsmTileLoader() {}

void OsmTileLoader::beginLoad(int z, int xmin, int xmax, int ymin, int ymax) {
  auto rm_cond{[z, xmin, xmax, ymin, ymax](const Tile &tile) {
    const bool c1{tile.zxy.at(0) != z};
    const bool c2{tile.zxy.at(1) < xmin || tile.zxy.at(1) > xmax};
    const bool c3{tile.zxy.at(2) < ymin || tile.zxy.at(2) > ymax};
    const bool c4{tile.future.valid() &&
                  tile.future.wait_for(0s) == std::future_status::ready};
    const bool c5{tile.texture};

    return (c1 || c2 || c3) && (c4 || c5);
  }};

  _tiles.erase(std::remove_if(_tiles.begin(), _tiles.end(), rm_cond),
               _tiles.end());

  _futureCounter = 0;
}

ImTextureID OsmTileLoader::tileAt(int z, int x, int y) {
  auto it{std::find(_tiles.begin(), _tiles.end(),
                    Tile{std::array<int, 3>{z, x, y}})};

  if (it == _tiles.end()) {
    if (_futureCounter++ < _futureLimit) {
      _tiles.insert(
          it, {{z, x, y},
               std::async(std::launch::async, &OsmTileLoader::onHandleRequest,
                          this, std::array<int, 3>({z, x, y}))});
    }
    return _blankTile.imID();
  }

  if (it->texture) {
    return it->texture.get()->imID();
  }

  if (!it->future.valid()) {
    return _blankTile.imID();
  }

  if (it->future.wait_for(0s) != std::future_status::ready) {
    return _blankTile.imID();
  }

  auto remote{it->future.get()};

  if (remote.code != CURLE_OK) {
    it = _tiles.erase(it);
    return _blankTile.imID();
  }

  if (_textureCounter++ < _textureLimit) {
    _textureCounter--;
    it->texture = remote.texture;
    return it->texture.get()->imID();
  }

  return _blankTile.imID();
}

OsmTileLoader::RemoteTile
OsmTileLoader::onHandleRequest(const std::array<int, 3> &zxy) {

  std::ostringstream urlmaker;
  urlmaker << _tileProvider << zxy[0] << '/' << zxy[1] << '/' << zxy[2]
           << _tileExtension;
  const auto url{urlmaker.str()};

  RemoteTile tile;
  CURL *curl{curl_easy_init()};
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
  // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, _clientName.c_str());
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&tile);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onPullResponse);
  tile.code = curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  if (tile.code == CURLE_OK) {
    tile.texture = std::make_shared<OsmTileTexture>(_tileSizePx, tile.blob);
  }

  // test for async
  // std::this_thread::sleep_for(1s);

  return tile;
}

size_t OsmTileLoader::onPullResponse(void *data, size_t size, size_t nmemb,
                                     void *userp) {
  size_t realsize{size * nmemb};
  auto &tile{*static_cast<RemoteTile *>(userp)};
  auto const *const dataptr{static_cast<std::byte *>(data)};
  tile.blob.insert(tile.blob.cend(), dataptr, dataptr + realsize);

  return realsize;
}
