#include "ImOsmTileGrabberUrl.h"
#include "ImOsmCoords.h"
#include <algorithm>
#include <fstream>
#include <future>
#include <sstream>
#include <thread>
using namespace std::chrono_literals;

namespace ImOsm {
namespace Old {
TileGrabberUrl::TileGrabberUrl() {}

void TileGrabberUrl::grab(int minZ, int maxZ, float minLon, float maxLon,
                          float minLat, float maxLat) {
  _grabFuture = std::async(std::launch::async, &TileGrabberUrl::onLaunchGrab,
                           this, minZ, maxZ, minLon, maxLon, minLat, maxLat);
}

TileGrabberUrl::GrabData
TileGrabberUrl::onLaunchGrab(int minZ, int maxZ, float minLon, float maxLon,
                             float minLat, float maxLat) {
  auto rm_cond{[this](Tile &tile) {
    const bool ready{tile.future.wait_for(0s) == std::future_status::ready};
    if (ready) {
      saveFile(tile);
    }
    return ready;
  }};

  GrabData data;
  for (auto z{minZ}; z != maxZ + 1; ++z) {
    const auto tilesNum{(1 << z)};
    const std::pair<int, int> tx{
        std::minmax(std::clamp(lon2tx(minLon, z), 0, tilesNum - 1),
                    std::clamp(lon2tx(maxLon, z), 0, tilesNum - 1))};
    const std::pair<int, int> ty{
        std::minmax(std::clamp(lat2ty(minLat, z), 0, tilesNum - 1),
                    std::clamp(lat2ty(maxLat, z), 0, tilesNum - 1))};

    // const auto total{(maxTX - minTX + 1) * (maxTY - minTY + 1)};
    for (auto x{tx.first}; x != tx.second + 1; ++x) {
      for (auto y{ty.first}; y != ty.second + 1; ++y) {
        data.tiles.push_back(
            {{z, x, y},
             std::async(std::launch::async, &TileGrabberUrl::onHandleRequest,
                        this, std::array<int, 3>({z, x, y}))});
        while (data.tiles.size() > _maxParallelGrab) {
          data.tiles.erase(
              std::remove_if(data.tiles.begin(), data.tiles.end(), rm_cond),
              data.tiles.end());
          std::this_thread::sleep_for(1s);
        }
      }
    }
  }
  while (data.tiles.size() > 0) {
    data.tiles.erase(
        std::remove_if(data.tiles.begin(), data.tiles.end(), rm_cond),
        data.tiles.end());
    std::this_thread::sleep_for(1s);
  }

  return data;
}

TileGrabberUrl::Tile::Remote
TileGrabberUrl::onHandleRequest(const std::array<int, 3> &zxy) {
  std::ostringstream urlmaker;
  urlmaker << _tileProvider << zxy[0] << '/' << zxy[1] << '/' << zxy[2]
           << _tileExtension;
  const auto url{urlmaker.str()};

  Tile::Remote tile;
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

  return tile;
}

size_t TileGrabberUrl::onPullResponse(void *data, size_t size, size_t nmemb,
                                      void *userp) {
  size_t realsize{size * nmemb};
  auto &tile{*static_cast<Tile::Remote *>(userp)};
  auto const *const dataptr{static_cast<std::byte *>(data)};
  tile.blob.insert(tile.blob.cend(), dataptr, dataptr + realsize);

  return realsize;
}

void TileGrabberUrl::saveFile(Tile &tile) {
  std::ostringstream pathmaker;
  pathmaker << _tileSavePath << '/' << tile.zxy[0] << '/' << tile.zxy[1];
  if (!std::filesystem::exists(pathmaker.str())) {
    std::filesystem::create_directories(pathmaker.str());
  }
  pathmaker << '/' << tile.zxy[0] << '-' << tile.zxy[1] << '-' << tile.zxy[2]
            << _tileExtension;
  std::ofstream stream(pathmaker.str().c_str(),
                       std::fstream::out | std::fstream::binary);
  if (stream) {
    const auto blob{tile.future.get().blob};
    stream.write(reinterpret_cast<const char *>(blob.data()), blob.size());
  }
}
} // namespace Old
} // namespace ImOsm
