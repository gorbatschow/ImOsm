#pragma once
#include "ImOsmTileSourceAsync.h"
#include <filesystem>

namespace ImOsm {
class TileSourceFs : public TileSourceAsync {
public:
  TileSourceFs(int requestLimit, bool preload,
               const std::filesystem::path &basePath);
  virtual ~TileSourceFs() = default;

  static std::string FileName(int z, int x, int y);
  static std::filesystem::path BasePathDefault();

protected:
  virtual bool receiveTile(int z, int x, int y, TileData &tileData) override;
  inline std::filesystem::path basePath() const { return _basePath; }

  virtual std::filesystem::path dirPath(int z, int x, int y) const = 0;

private:
  std::filesystem::path _basePath{BasePathDefault()};
};

// -----------------------------------------------------------------------------

class TileSourceFsDir : public TileSourceFs {
public:
  TileSourceFsDir(int requestLimit, bool preload,
                  const std::filesystem::path &basePath);
  virtual ~TileSourceFsDir() = default;

  static std::filesystem::path DirPath(std::filesystem::path basePath,
                                       [[maybe_unused]] int z,
                                       [[maybe_unused]] int x,
                                       [[maybe_unused]] int y) {
    return basePath;
  }

protected:
  virtual std::filesystem::path dirPath(int z, int x, int y) const override;
};

// -----------------------------------------------------------------------------

class TileSourceFsSubDir : public TileSourceFs {
public:
  TileSourceFsSubDir(int requestLimit, bool preload,
                     const std::filesystem::path &basePath);
  virtual ~TileSourceFsSubDir() = default;

  static std::filesystem::path DirPath(std::filesystem::path basePath, int z,
                                       [[maybe_unused]] int x,
                                       [[maybe_unused]] int y);

protected:
  virtual std::filesystem::path dirPath(int z, int x, int y) const override;
};

} // namespace ImOsm
