#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <curl/curl.h>
#include <filesystem>
#include <fstream>
#include <future>
#include <memory>
#include <sstream>
#include <vector>
using namespace std::chrono_literals;

namespace ImOsm {
class Tile {
public:
  Tile(int z, int x, int y, const std::vector<std::byte> &blob, bool ok = true)
      : _z{z}, _x{x}, _y{y}, _blob{blob}, _ok{ok} {}

  void loadTexture() {}

  void saveFile(const std::string &dirName, const std::string &ext = ".png") {
    std::ostringstream pathmaker;
    pathmaker << dirName << '/' << _z << '/' << _x;
    if (!std::filesystem::exists(pathmaker.str())) {
      std::filesystem::create_directories(pathmaker.str());
    }
    pathmaker << '/' << _z << '-' << _x << '-' << _y << ext;
    std::ofstream filemaker(pathmaker.str().c_str(),
                            std::fstream::out | std::fstream::binary);
    if (filemaker) {
      filemaker.write(reinterpret_cast<const char *>(_blob.data()),
                      _blob.size());
    }
  }

private:
  int _z{}, _x{}, _y{};
  std::vector<std::byte> _blob;
  bool _ok{};
};
// -----------------------------------------------------------------------------
class ITileSource {
public:
  virtual ~ITileSource() = default;

  virtual bool hasRequest() = 0;
  virtual bool canRequest() = 0;
  virtual void request(int z, int x, int y) = 0;
  virtual bool canTakeAll() = 0;
  virtual std::vector<std::shared_ptr<Tile>> takeAll() = 0;

  virtual const std::string &tileExtension() const = 0;
};
// -----------------------------------------------------------------------------
class TileSourceAsync : public ITileSource {
public:
  TileSourceAsync() = default;
  virtual ~TileSourceAsync() = default;

  void setRequestLimit() {}

  virtual bool hasRequest() override { return !_requests.empty(); }

  virtual bool canRequest() override {
    return _requests.size() < _requestLimit;
  }

  virtual bool canTakeAll() override {
    return std::all_of(
        std::begin(_requests), std::end(_requests), [](AsyncTile &tile) {
          return tile.future.wait_for(0s) == std::future_status::ready;
        });
  }

  virtual void request(int z, int x, int y) override {
    assert(canRequest());
    if (_requests.size() < _requestLimit) {
      _requests.push_back(
          {z, x, y,
           std::async(std::launch::async, &TileSourceAsync::onHandleRequest,
                      this, z, x, y)});
    }
  }

  virtual std::vector<std::shared_ptr<Tile>> takeAll() override {
    std::vector<std::shared_ptr<Tile>> tiles;
    for (auto &request : _requests) {
      tiles.push_back(request.makeTile());
    }
    return tiles;
  }

protected:
  struct AsyncTile {
    struct FutureData {
      std::vector<std::byte> blob;
      bool ok{};
    };

    int z{}, x{}, y{};
    std::future<FutureData> future;

    bool operator==(const AsyncTile &other) const {
      return this->z == other.z && this->x == other.x && this->y == other.y;
    }
    bool operator==(const std::array<int, 3> &zxy) const {
      return this->z == zxy[0] && this->x == zxy[1] && this->y == zxy[2];
    }

    std::shared_ptr<Tile> makeTile() {
      assert(future.wait_for(0s) == std::future_status::ready);
      return std::make_shared<Tile>(z, x, y, future.get().blob,
                                    future.get().ok);
    }
  };
  std::vector<AsyncTile> _requests;
  int _requestLimit{1};

  virtual AsyncTile::FutureData onHandleRequest(int z, int x, int y) = 0;
};
// -----------------------------------------------------------------------------
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

protected:
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

private:
  std::string _clientName{"curl"};
  std::string _tileProvider{"http://a.tile.openstreetmap.org/"};
  std::string _tileExtension{".png"};
};

} // namespace ImOsm
