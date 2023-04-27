#include "ImOsmTileSourceUrl.h"
#include <curl/curl.h>

namespace ImOsm {

size_t onWrite(void *data, size_t size, size_t nmemb, void *userp);
size_t onProgress(void *clientp, double dltotal, double dlnow, double ultotal,
                  double ulnow);
curl_socket_t onOpenSocket(void *clientp, curlsocktype purpose,
                           struct curl_sockaddr *address);

TileSourceUrl::TileSourceUrl(int requestLimit, bool preload,
                             const std::string &userAgent)
    : TileSourceAsync{requestLimit, preload}, _userAgent{userAgent} {}

TileSourceUrl::~TileSourceUrl() {}

bool TileSourceUrl::receiveTile(int z, int x, int y, TileData &tileData) {
  CURL *curl{curl_easy_init()};
  curl_easy_setopt(curl, CURLOPT_URL, makeUrl(z, x, y).c_str());
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
  // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, _userAgent.c_str());
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1);
  // Write
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onWrite);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&tileData);
  // Progress
  curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, onProgress);
  curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &tileData);
  // Open Socket
  curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, onOpenSocket);
  curl_easy_setopt(curl, CURLOPT_OPENSOCKETDATA, &tileData);
  const bool ok{curl_easy_perform(curl) == CURLE_OK};
  curl_easy_cleanup(curl);

  return ok;
}

size_t onWrite(void *data, size_t size, size_t nmemb, void *userp) {
  auto &tileData{*static_cast<TileSourceAsync::TileData *>(userp)};
  auto &blob{tileData.blob};
  size_t realsize{size * nmemb};
  auto const *const dataptr{static_cast<std::byte *>(data)};
  blob.insert(blob.cend(), dataptr, dataptr + realsize);
  return realsize;
}

size_t onProgress(void *clientp, double dltotal, double dlnow, double ultotal,
                  double ulnow) {
  auto &tileData{*static_cast<TileSourceAsync::TileData *>(clientp)};
  return 0;
}

curl_socket_t onOpenSocket(void *clientp, curlsocktype purpose,
                           struct curl_sockaddr *address) {
  auto &tileData{*static_cast<TileSourceAsync::TileData *>(clientp)};
  auto s = socket(address->family, address->socktype, address->protocol);
  return s;
}

} // namespace ImOsm
