#pragma once
#include <memory>
#include <vector>

namespace ImOsm {
class ITile;
class ITileSaver;

class ITileSource {
public:
  virtual ~ITileSource() = default;

  virtual bool hasRequest() = 0;
  virtual bool hasRequest(int z, int x, int y) = 0;
  virtual bool canRequest() = 0;
  virtual bool request(int z, int x, int y) = 0;

  virtual void waitAll() = 0;
  virtual bool canTakeAll() = 0;
  virtual void takeAll(std::vector<std::shared_ptr<ITile>> &tiles) = 0;
  virtual int takeReady(std::vector<std::shared_ptr<ITile>> &tiles) = 0;
  virtual bool saveAll(std::shared_ptr<ITileSaver> saver) = 0;

  virtual bool preload() const = 0;
  virtual int requestLimit() const = 0;
};

} // namespace ImOsm
