#pragma once
#include "ImOsmTileDummy.h"
#include <GL/gl.h>
#include <string>
#include <vector>

namespace ImOsm {
class Tile : public TileDummy {
public:
  Tile(int z, int x, int y, const std::vector<std::byte> &rawBlob,
       bool preload = true);
  virtual ~Tile();

  virtual const char *rawBlob() const override {
    return reinterpret_cast<const char *>(_rawBlob.data());
  }

  virtual std::size_t rawBlobSize() const override { return _rawBlob.size(); }

  virtual void rgbaLoad() const override { stbLoad(); }

  virtual const char *rgbaBlob() const override {
    return reinterpret_cast<const char *>(_rgbaBlob.data());
  }

  virtual std::size_t rgbaBlobSize() const override { return _rgbaBlob.size(); }

  virtual ImTextureID texture() const override {
    return (ImTextureID)(intptr_t)glID();
  }

private:
  GLuint glID() const;
  void stbLoad() const;
  void glLoad() const;

  std::vector<std::byte> _rawBlob;

  mutable int _pxW{}, _pxH{};
  mutable int _channels{};
  mutable std::vector<std::byte> _rgbaBlob;
  mutable GLuint _id{0};
};
} // namespace ImOsm
