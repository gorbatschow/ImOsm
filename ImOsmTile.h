#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace ImOsm {
class Tile {
public:
  Tile(int z, int x, int y, const std::vector<std::byte> &blob, bool ok = true);

  void loadTexture();

  void saveFile(const std::string &dirName,
                const std::string &ext = ".png") const;

private:
  int _z{}, _x{}, _y{};
  std::vector<std::byte> _blob;
  bool _ok{};
};
} // namespace ImOsm
