#pragma once
#include "ImOsmITileSaver.h"
#include "ImOsmITileSource.h"
#include "ImOsmTileAsync.h"
#include <algorithm>
#include <array>
#include <future>
#include <thread>

namespace ImOsm {
using namespace std::chrono_literals;

class TileSourceAsync : public ITileSource {
public:
  TileSourceAsync(int requestLimit = 10, bool preload = true);
  virtual ~TileSourceAsync() = default;

  virtual bool hasRequest() override;
  virtual bool hasRequest(int z, int x, int y) override;
  virtual bool canRequest() override;
  virtual bool request(int z, int x, int y) override;
  virtual void waitAll() override;
  virtual bool canTakeAll() override;
  virtual void takeAll(std::vector<std::shared_ptr<ITile>> &tiles) override;
  virtual int takeReady(std::vector<std::shared_ptr<ITile>> &tiles) override;
  virtual bool saveAll(std::shared_ptr<ITileSaver> saver) override;

protected:
  virtual bool receiveTile(int z, int x, int y,
                           std::vector<std::byte> &blob) = 0;

private:
  TileAsync::FutureData onHandleRequest(int z, int x, int y);

  std::vector<TileAsync> _requests;
  int _requestLimit{10};
  bool _preload{true};
};
} // namespace ImOsm
