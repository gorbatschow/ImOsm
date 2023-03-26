#pragma once
#include "ImOsmITile.h"
#include "ImOsmITileSaver.h"
#include <filesystem>
#include <fstream>
#include <string>

namespace ImOsm {
class TileSaver : public ITileSaver {
public:
  TileSaver() {}
  virtual ~TileSaver() = default;

  virtual bool
  saveMulti(const std::vector<std::shared_ptr<ITile>> &tiles) const override {
    for (const auto &tile : tiles) {
      if (!save(tile)) {
        return false;
      }
    }
    return true;
  }

  virtual bool save(std::shared_ptr<ITile> tile) const override {
    auto path{dirPath(tile)};
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    std::ofstream file_maker(path.append(fileName(tile)),
                             std::fstream::out | std::fstream::binary);
    if (file_maker) {
      file_maker.write(tile->rawBlob(), tile->rawBlobSize());
      return true;
    }
    return false;
  }

protected:
  virtual std::filesystem::path dirPath(std::shared_ptr<ITile>) const = 0;
  virtual std::string fileName(std::shared_ptr<ITile>) const = 0;
};

// -----------------------------------------------------------------------------

class TileSaverDir : public TileSaver {
public:
  TileSaverDir() {}
  TileSaverDir(const std::filesystem::path &dirPath) : _dirPath{dirPath} {}
  virtual ~TileSaverDir() = default;

protected:
  virtual std::filesystem::path dirPath(std::shared_ptr<ITile>) const override {
    return _dirPath;
  }

  virtual std::string fileName(std::shared_ptr<ITile> tile) const override {
    std::ostringstream fname_maker;
    fname_maker << tile->z() << '-' << tile->x() << '-' << tile->y();
    return fname_maker.str();
  }

private:
  std::filesystem::path _dirPath{
      std::filesystem::current_path().append("tiles")};
};

// -----------------------------------------------------------------------------

class TileSaverSubDir {};

} // namespace ImOsm
