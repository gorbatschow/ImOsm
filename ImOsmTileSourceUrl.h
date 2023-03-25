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
  virtual bool receiveTile(int z, int x, int y,
                           std::vector<std::byte> &blob) override {
    TileAsync::FutureData tile;
    CURL *curl{curl_easy_init()};
    curl_easy_setopt(curl, CURLOPT_URL, makeUrl(z, x, y).c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, _userAgent.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&blob);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onPullResponse);
    const bool ok{curl_easy_perform(curl) == CURLE_OK};
    curl_easy_cleanup(curl);

    return ok;
  }

  virtual std::string makeUrl(int z, int x, int y) = 0;

private:
  static size_t onPullResponse(void *data, size_t size, size_t nmemb,
                               void *userp) {
    size_t realsize{size * nmemb};
    auto &blob{*static_cast<std::vector<std::byte> *>(userp)};
    auto const *const dataptr{static_cast<std::byte *>(data)};
    blob.insert(blob.cend(), dataptr, dataptr + realsize);
    return realsize;
  }

  std::string _userAgent{"curl"};
};
} // namespace ImOsm
