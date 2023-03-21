#include "ImOsmTile.h"

ImOsm::Tile::Tile(int z, int x, int y, const std::vector<std::byte> &blob,
                  TileFormat format, bool ok)
    : _z{z}, _x{x}, _y{y}, _blob{blob}, _ok{ok} {}
