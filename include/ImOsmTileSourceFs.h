#pragma once
#include "ImOsmTileSourceAsync.h"
#include <filesystem>
#include <fstream>

namespace ImOsm {
class TileSourceFs : public TileSourceAsync {
public:
  TileSourceFs(int requestLimit, bool preload,
               const std::filesystem::path &basePath)
      : TileSourceAsync{requestLimit, preload}, _basePath{basePath} {}
  virtual ~TileSourceFs() = default;

  static std::string FileName(int z, int x, int y) {
    std::ostringstream fname_maker;
    fname_maker << z << '-' << x << '-' << y;
    return fname_maker.str();
  }

  static std::filesystem::path BasePathDefault() {
    return std::filesystem::current_path().append("tiles");
  }

protected:
  virtual bool receiveTile(int z, int x, int y,
                           std::vector<std::byte> &blob) override {
    auto path{dirPath(z, x, y)};
    std::ifstream reader(path.append(FileName(z, x, y)),
                         std::fstream::in | std::fstream::binary |
                             std::ios::ate);
    auto pos{reader.tellg()};
    if (reader && pos > 0) {
      blob.resize(pos);
      reader.seekg(0, std::ios::beg);
      reader.read(reinterpret_cast<char *>(&blob[0]), pos);
      return true;
    }
    return false;
  }

  virtual std::filesystem::path dirPath(int z, int x, int y) const = 0;

  inline std::filesystem::path basePath() const { return _basePath; }

private:
  std::filesystem::path _basePath{BasePathDefault()};
};

// -----------------------------------------------------------------------------

class TileSourceFsDir : public TileSourceFs {
public:
  TileSourceFsDir(int requestLimit, bool preload,
                  const std::filesystem::path &basePath)
      : TileSourceFs{requestLimit, preload, basePath} {}
  virtual ~TileSourceFsDir() = default;

  static std::filesystem::path DirPath(std::filesystem::path basePath,
                                       [[maybe_unused]] int z,
                                       [[maybe_unused]] int x,
                                       [[maybe_unused]] int y) {
    return basePath;
  }

protected:
  virtual std::filesystem::path dirPath(int z, int x, int y) const override {
    return DirPath(basePath(), z, x, y);
  }
};

// -----------------------------------------------------------------------------

class TileSourceFsSubDir : public TileSourceFs {
public:
  TileSourceFsSubDir(int requestLimit, bool preload,
                     const std::filesystem::path &basePath)
      : TileSourceFs{requestLimit, preload, basePath} {}
  virtual ~TileSourceFsSubDir() = default;

  static std::filesystem::path DirPath(std::filesystem::path basePath, int z,
                                       [[maybe_unused]] int x,
                                       [[maybe_unused]] int y) {
    return basePath.append(std::to_string(z));
  }

protected:
  virtual std::filesystem::path dirPath(int z, int x, int y) const override {
    return DirPath(basePath(), z, x, y);
  }
};

} // namespace ImOsm
