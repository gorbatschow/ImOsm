#include "ImOsmTileSourceAsync.h"
#include "ImOsmTile.h"

namespace ImOsm {
TileSourceAsync::TileSourceAsync(int requestLimit, bool preload)
    : _requestLimit{requestLimit}, _preload{preload} {}

bool TileSourceAsync::hasRequest() { return !_requests.empty(); }

bool TileSourceAsync::hasRequest(int z, int x, int y) {
  const auto it{std::find(_requests.begin(), _requests.end(),
                          std::array<int, 3>{z, x, y})};
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

bool TileSourceAsync::canTakeAll() {
  return std::all_of(std::begin(_requests), std::end(_requests),
                     [](TileAsync &tile) { return tile.ready(); });
}

bool TileSourceAsync::takeAll(std::vector<std::shared_ptr<ITile>> &tiles) {
  if (canTakeAll()) {
    for (auto &request : _requests) {
      tiles.push_back(request.future.get().tile);
    }
    _requests.clear();
    return true;
  }
  return false;
}

bool TileSourceAsync::takeReady(std::vector<std::shared_ptr<ITile>> &tiles) {
  const bool sz = _requests.size();
  for (auto it{_requests.begin()}; it != _requests.end();) {
    if (it->ready()) {
      tiles.push_back((*it).future.get().tile);
      it = _requests.erase(it);
    } else {
      ++it;
    }
  }
  return sz != _requests.size() && sz != 0;
}

bool TileSourceAsync::saveAll(std::shared_ptr<ITileSaver> saver) {
  std::vector<std::shared_ptr<ITile>> tiles;
  while (!takeAll(tiles)) {
    std::this_thread::sleep_for(1s);
  }
  return saver->saveMulti(tiles);
}

TileAsync::FutureData TileSourceAsync::onHandleRequest(int z, int x, int y) {
  std::vector<std::byte> blob;
  TileAsync::FutureData data;
  if (receiveTile(z, x, y, blob)) {
    data.tile = std::make_shared<Tile>(z, x, y, blob, _preload);
  } else {
    data.tile = std::make_shared<TileDummy>(z, x, y);
  }
  return data;
}
} // namespace ImOsm
