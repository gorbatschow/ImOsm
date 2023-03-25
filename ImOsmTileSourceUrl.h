#pragma once
#include "ImOsmTileSourceAsync.h"
#include <curl/curl.h>
#include <ostream>
#include <sstream>

namespace ImOsm {
class TileSourceUrl : public TileSourceAsync {
public:
  TileSourceUrl() = default;
  virtual ~TileSourceUrl() = default;

  inline void setUserAgent(const std::string &name) { _userAgent = name; }
  inline const std::string &userAgent() const { return _userAgent; }

protected:
  virtual TileAsync::FutureData onHandleRequest(int z, int x, int y) override {

    const auto url{makeUrl(z, x, y)};

    typename TileAsync::FutureData tile;
    CURL *curl{curl_easy_init()};
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, _userAgent.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&tile);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onPullResponse);
    tile.ok = curl_easy_perform(curl) == CURLE_OK;
    curl_easy_cleanup(curl);

    return tile;
  }

  virtual std::string makeUrl(int z, int x, int y) = 0;

private:
  static size_t onPullResponse(void *data, size_t size, size_t nmemb,
                               void *userp) {
    size_t realsize{size * nmemb};
    auto &tile{*static_cast<typename TileAsync::FutureData *>(userp)};
    auto const *const dataptr{static_cast<std::byte *>(data)};
    tile.blob.insert(tile.blob.cend(), dataptr, dataptr + realsize);

    return realsize;
  }

  std::string _userAgent{"curl"};
};
} // namespace ImOsm
