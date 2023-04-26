#include "ImOsmTileSaver.h"
#include "ImOsmITile.h"
#include <fstream>
#include <string>

namespace ImOsm {

TileSaver::TileSaver() = default;
TileSaver::~TileSaver() = default;

bool TileSaver::saveMulti(
    const std::vector<std::shared_ptr<ITile>> &tiles) const {
  for (const auto &tile : tiles) {
    if (!save(tile)) {
      return false;
    }
  }
  return true;
}

bool TileSaver::save(std::shared_ptr<ITile> tile) const {
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

std::string TileSaver::fileName(std::shared_ptr<ITile> tile) const {
  return TileSourceFs::FileName(tile->z(), tile->x(), tile->y());
}
// -----------------------------------------------------------------------------

TileSaverDir::TileSaverDir() = default;

TileSaverDir::TileSaverDir(const std::filesystem::path &basePath)
    : _basePath{basePath} {}

std::filesystem::path TileSaverDir::dirPath(std::shared_ptr<ITile> tile) const {
  return TileSourceFsDir::DirPath(_basePath, tile->z(), tile->x(), tile->y());
}

TileSaverDir::~TileSaverDir() = default;

// -----------------------------------------------------------------------------

TileSaverSubDir::TileSaverSubDir() = default;

TileSaverSubDir::TileSaverSubDir(const std::filesystem::path &basePath)
    : _basePath{basePath} {}

TileSaverSubDir::~TileSaverSubDir() = default;

std::filesystem::path
TileSaverSubDir::dirPath(std::shared_ptr<ITile> tile) const {
  return TileSourceFsSubDir::DirPath(_basePath, tile->z(), tile->x(),
                                     tile->y());
}

} // namespace ImOsm
