#include "ImOsmTile.h"
#include <GL/gl.h>
#include <stb_image.h>

namespace ImOsm {
ImOsm::Tile::Tile(int z, int x, int y, const std::vector<std::byte> &rawBlob,
                  bool preload)
    : TileDummy(z, x, y), _rawBlob{rawBlob} {
  if (preload) {
    stbLoad();
  }
}

Tile::~Tile() {
  if (_id != 0) {
    glDeleteTextures(1, &_id);
  }
}

const char *Tile::rawBlob() const {
  return reinterpret_cast<const char *>(_rawBlob.data());
}

std::size_t Tile::rawBlobSize() const { return _rawBlob.size(); }

const char *Tile::rgbaBlob() const {
  return reinterpret_cast<const char *>(_rgbaBlob.data());
}

std::size_t Tile::rgbaBlobSize() const { return _rgbaBlob.size(); }

ImTextureID Tile::texture() const {
  return (ImTextureID)(intptr_t)glID();
}

GLuint Tile::glID() const {
  if (_id == 0) {
    if (_rgbaBlob.empty()) {
        stbLoad();
    }
    glLoad();
  }
  return _id;
}

void Tile::stbLoad() const {
  stbi_set_flip_vertically_on_load(false);
  const auto ptr{
      stbi_load_from_memory(reinterpret_cast<stbi_uc const *>(_rawBlob.data()),
                            static_cast<int>(_rawBlob.size()), &_pxW, &_pxH,
                            &_channels, STBI_rgb_alpha)};
  if (ptr) {
    const std::size_t nbytes{std::size_t(_pxW * _pxH * STBI_rgb_alpha)};
    _rgbaBlob.resize(nbytes);
    _rgbaBlob.shrink_to_fit();
    const auto byteptr{reinterpret_cast<std::byte *>(ptr)};
    _rgbaBlob.insert(_rgbaBlob.begin(), byteptr, byteptr + nbytes);
    stbi_image_free(ptr);
  }
}

void Tile::glLoad() const {
  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _pxW, _pxH, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, rgbaBlob());
}
}; // namespace ImOsm
