#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace ImOsm {
class Tile {
public:
  Tile(int z, int x, int y, const std::vector<std::byte> &blob, bool ok = true)
      : _z{z}, _x{x}, _y{y}, _blob{blob}, _ok{ok} {}

  void loadTexture() {}

  void saveFile(const std::string &dirName,
                const std::string &ext = ".png") const {
    std::ostringstream pathmaker;
    pathmaker << dirName << '/' << _z << '/' << _x;
    if (!std::filesystem::exists(pathmaker.str())) {
      std::filesystem::create_directories(pathmaker.str());
    }
    pathmaker << '/' << _z << '-' << _x << '-' << _y << ext;
    std::ofstream filemaker(pathmaker.str().c_str(),
                            std::fstream::out | std::fstream::binary);
    if (filemaker) {
      filemaker.write(reinterpret_cast<const char *>(_blob.data()),
                      _blob.size());
    }
  }

private:
  int _z{}, _x{}, _y{};
  std::vector<std::byte> _blob;
  bool _ok{};
};
} // namespace ImOsm
