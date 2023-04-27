#pragma once
#include "ImOsmITileSource.h"
#include "ImOsmTileAsync.h"

namespace ImOsm {
class ITile;
class ITileSaver;

class TileSourceAsync : public ITileSource {
public:
  TileSourceAsync(int requestLimit, bool preload);
  virtual ~TileSourceAsync();

  virtual bool hasRequest() override;
  virtual bool hasRequest(int z, int x, int y) override;
  virtual bool canRequest() override;
  virtual bool request(int z, int x, int y) override;

  virtual void waitAll() override;
  virtual bool canTakeAll() override;
  virtual void takeAll(std::vector<std::shared_ptr<ITile>> &tiles) override;
  virtual int takeReady(std::vector<std::shared_ptr<ITile>> &tiles) override;
  virtual bool saveAll(std::shared_ptr<ITileSaver> saver) override;

  virtual bool preload() const override { return _preload; }
  virtual int requestLimit() const override { return _requestLimit; }

  struct TileData {
    std::atomic_bool &interrupt;
    std::vector<std::byte> blob;
  };

protected:
  virtual bool receiveTile(int z, int x, int y, TileData &data) = 0;

private:
  TileAsync::FutureData onHandleRequest(int z, int x, int y);

  std::vector<TileAsync> _requests;
  int _requestLimit{10};
  bool _preload{true};
  std::atomic_bool _interrupt;
};
} // namespace ImOsm
