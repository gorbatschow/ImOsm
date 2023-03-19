#pragma once
#include "ImOsmTileSourceAsync.h"
#include <curl/curl.h>

namespace ImOsm {
class TileSourceUrl : public TileSourceAsync {
public:
  TileSourceUrl() = default;
  virtual ~TileSourceUrl() = default;

  virtual const std::string &tileExtension() const override {
    return _tileExtension;
  }

  inline void setTileProvider(const std::string &url) { _tileProvider = url; }
  inline const std::string &tileProvider() const { return _tileProvider; }

  inline void setTileExtension(const std::string &ext) { _tileExtension = ext; }
  inline const std::string &tileExtensioin() const { return _tileExtension; }

  inline void setClientName(const std::string &name) { _clientName = name; }
  inline const std::string &clientName() const { return _clientName; }

private:
  AsyncTile::FutureData onHandleRequest(int z, int x, int y) override {
    std::ostringstream urlmaker;
    urlmaker << _tileProvider << z << '/' << x << '/' << y << _tileExtension;
    const auto url{urlmaker.str()};

    AsyncTile::FutureData tile;
    CURL *curl{curl_easy_init()};
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, _clientName.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&tile);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onPullResponse);
    tile.ok = curl_easy_perform(curl) == CURLE_OK;
    curl_easy_cleanup(curl);

    return tile;
  }

  static size_t onPullResponse(void *data, size_t size, size_t nmemb,
                               void *userp) {
    size_t realsize{size * nmemb};
    auto &tile{*static_cast<AsyncTile::FutureData *>(userp)};
    auto const *const dataptr{static_cast<std::byte *>(data)};
    tile.blob.insert(tile.blob.cend(), dataptr, dataptr + realsize);

    return realsize;
  }

  std::string _clientName{"curl"};
  std::string _tileProvider{"http://a.tile.openstreetmap.org/"};
  std::string _tileExtension{".png"};
};
} // namespace ImOsm
