#include "ImOsmTileTexture.h"
#include <cassert>
#include <cmath>

namespace ImOsm {
namespace Old {
TileTexture::TileTexture(int size, TextureColor color)
    : _width(size), _height(size) {

  _blob.resize(_width * _height * TextureColor::RGBA_SZ);
  _blob.shrink_to_fit();
  for (size_t i = 0; i != _blob.size(); i = i + TextureColor::RGBA_SZ) {
    _blob[i] = std::byte(color.rgba[0]);
    _blob[i + 1] = std::byte(color.rgba[1]);
    _blob[i + 2] = std::byte(color.rgba[2]);
    _blob[i + 3] = std::byte(color.rgba[3]);
  }
}

TileTexture::TileTexture(int size, const std::vector<std::byte> &blob) {
  stbi_set_flip_vertically_on_load(false);
  const auto ptr{
      stbi_load_from_memory(reinterpret_cast<stbi_uc const *>(blob.data()),
                            static_cast<int>(blob.size()), &_width, &_height,
                            &_channels, STBI_rgb_alpha)};
  if (ptr) {
    const auto byteptr{reinterpret_cast<std::byte *>(ptr)};
    _blob.insert(_blob.begin(), byteptr,
                 byteptr + _width * _height * STBI_rgb_alpha);
    stbi_image_free(ptr);
  }
}

TileTexture::~TileTexture() { glDeleteTextures(1, &_id); }

void TileTexture::loadTexture() const {
  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, reinterpret_cast<const char *>(_blob.data()));
}
} // namespace Old
} // namespace ImOsm
