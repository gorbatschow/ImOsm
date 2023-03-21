#pragma once
#include "ImOsmITile.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace ImOsm {
class Tile : public ITile {
public:
  Tile(int z, int x, int y, const std::vector<std::byte> &blob,
       TileFormat ext = TileFormat::PNG, bool ok = true);
  virtual ~Tile() = default;

  void loadTexture() override {}

  virtual int z() const override { return _z; }
  virtual int x() const override { return _x; }
  virtual int y() const override { return _y; }
  virtual const std::vector<std::byte> &blob() const override { return _blob; }

private:
  int _z{}, _x{}, _y{};
  std::vector<std::byte> _blob;
  bool _ok{};
};
} // namespace ImOsm
