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
  saveMulti(const std::vector<std::shared_ptr<ITile>> &tiles) const {
    for (std::shared_ptr<ITile> tile : tiles) {
      if (!save(tile)) {
        return false;
      }
    }
    return true;
  }
};

class TileSaverDir : public TileSaver {
public:
  TileSaverDir() {}
  TileSaverDir(const std::string &dname) : _dname{dname} {}
  virtual ~TileSaverDir() = default;

  virtual bool save(std::shared_ptr<ITile> tile) const override {
    if (!std::filesystem::exists(_dname)) {
      std::filesystem::create_directories(_dname);
    }

    std::ostringstream fname_maker;
    fname_maker << _dname << '/' << tile->z() << '-' << tile->x() << '-'
                << tile->y();

    std::ofstream file_maker(fname_maker.str().c_str(),
                             std::fstream::out | std::fstream::binary);
    if (file_maker) {
      file_maker.write(tile->rawBlob(), tile->rawBlobSize());
      return true;
    }
    return false;
  }

private:
  std::string _dname{std::filesystem::current_path().string() + "/tiles/"};
};

class TileSaverSubDir {};

} // namespace ImOsm
