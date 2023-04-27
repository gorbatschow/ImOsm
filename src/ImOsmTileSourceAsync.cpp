#include "ImOsmTileSourceAsync.h"
#include "ImOsmITileSaver.h"
#include "ImOsmTile.h"
#include "ImOsmTileAsync.h"
#include <algorithm>

namespace ImOsm {
using namespace std::chrono_literals;

TileSourceAsync::TileSourceAsync(int requestLimit, bool preload)
    : _requestLimit{requestLimit}, _preload{preload} {}

TileSourceAsync::~TileSourceAsync() { _interrupt = true; }

bool TileSourceAsync::hasRequest() { return !_requests.empty(); }

bool TileSourceAsync::hasRequest(int z, int x, int y) {
  const auto it{
      std::find(_requests.begin(), _requests.end(), TileAsync{z, x, y})};
  return it != _requests.end();
}

bool TileSourceAsync::canRequest() { return _requests.size() < _requestLimit; }

bool TileSourceAsync::request(int z, int x, int y) {
  if (canRequest()) {
    _requests.push_back(TileAsync{z, x, y,
                                  std::async(std::launch::async,
                                             &TileSourceAsync::onHandleRequest,
                                             this, z, x, y)});
    return true;
  }
  return false;
}

void TileSourceAsync::waitAll() {
  while (!canTakeAll()) {
    std::this_thread::sleep_for(1s);
  }
}

bool TileSourceAsync::canTakeAll() {
  return std::all_of(std::begin(_requests), std::end(_requests),
                     [](TileAsync &tile) { return tile.ready(); });
}

void TileSourceAsync::takeAll(std::vector<std::shared_ptr<ITile>> &tiles) {
  waitAll();
  for (auto &request : _requests) {
    tiles.push_back(request.future.get().tile);
  }
  _requests.clear();
}

int TileSourceAsync::takeReady(std::vector<std::shared_ptr<ITile>> &tiles) {
  int taked{};
  for (auto it{_requests.begin()}; it != _requests.end();) {
    if (it->ready()) {
      tiles.push_back((*it).future.get().tile);
      it = _requests.erase(it);
      ++taked;
    } else {
      ++it;
    }
  }
  return taked;
}

bool TileSourceAsync::saveAll(std::shared_ptr<ITileSaver> saver) {
  std::vector<std::shared_ptr<ITile>> tiles;
  takeAll(tiles);
  return saver->saveMulti(tiles);
}

TileAsync::FutureData TileSourceAsync::onHandleRequest(int z, int x, int y) {
  TileData tileData{_interrupt};
  TileAsync::FutureData futureData;
  if (receiveTile(z, x, y, tileData)) {
    futureData.tile = std::make_shared<Tile>(z, x, y, tileData.blob, _preload);
  } else {
    futureData.tile = std::make_shared<TileDummy>(z, x, y);
  }
  return futureData;
}
} // namespace ImOsm
