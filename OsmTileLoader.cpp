#include "OsmTileLoader.h"
#include <GL/gl.h>
#include <algorithm>
#include <curl/curl.h>
#include <future>
#include <iostream>
#include <memory.h>
#include <sstream>

OsmTileLoader::OsmTileLoader() {}

OsmTileLoader::~OsmTileLoader() {}

void OsmTileLoader::beginLoad(int z, int xmin, int xmax, int ymin, int ymax) {
  auto rm_cond = [z, xmin, xmax, ymin, ymax](const Tile &tile) {
    return (tile.zxy.at(0) != z || tile.zxy.at(1) < xmin ||
            tile.zxy.at(1) > xmax || tile.zxy.at(2) < ymin ||
            tile.zxy.at(2) > ymax);
  };
  _tiles.erase(std::remove_if(_tiles.begin(), _tiles.end(), rm_cond),
               _tiles.end());

  _futureCounter = 0;
  //_textureCounter = 0;
}

ImTextureID OsmTileLoader::tileAt(int z, int x, int y) {
  auto it = std::find(_tiles.begin(), _tiles.end(),
                      Tile{std::array<int, 3>{z, x, y}});

  if (it == _tiles.end()) {
    if (_futureCounter++ < _futureLimit) {
      auto future =
          std::async(std::launch::async, &OsmTileLoader::onHandleRequest, this,
                     std::array<int, 3>({z, x, y}));
      _tiles.insert(it, Tile{{z, x, y}, std::move(future)});
    }
    return _blankTile.imID();
  }

  if (it->texture) {
    return it->texture.get()->imID();
  }

  if (it->future.valid()) {
    if (it->future.get().code != CURLE_OK) {
      it = _tiles.erase(it);
      return _blankTile.imID();
    }

    if (_textureCounter++ < _textureLimit) {
      auto ptr =
          std::const_pointer_cast<OsmTileTexture>(it->future.get().texture);
      it->texture.swap(ptr);
      it->texture->loadTexture();
      _textureCounter--;
      return it->texture.get()->imID();
    }
  }

  return _blankTile.imID();
}

OsmTileLoader::RemoteTile
OsmTileLoader::onHandleRequest(const std::array<int, 3> &zxy) {

  std::ostringstream urlmaker;
  urlmaker << "http://a.tile.openstreetmap.org/" << zxy[0] << "/" << zxy[1]
           << "/" << zxy[2] << ".png";
  const auto url = urlmaker.str();

  RemoteTile tile;
  CURL *curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
  // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.84.0");
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&tile);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onPullResponse);
  tile.code = curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  tile.texture = std::make_shared<OsmTileTexture>(_tileSizePx, tile.blob);

  return tile;
}

size_t OsmTileLoader::onPullResponse(void *data, size_t size, size_t nmemb,
                                     void *userp) {
  size_t realsize = size * nmemb;
  auto &tile = *static_cast<RemoteTile *>(userp);
  auto const *const dataptr = static_cast<std::byte *>(data);
  tile.blob.insert(tile.blob.cend(), dataptr, dataptr + realsize);

  return realsize;
}
