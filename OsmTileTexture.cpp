#include "OsmTileTexture.h"
#include <cassert>
#include <cmath>

OsmTileTexture::OsmTileTexture(int size, TextureColor color)
    : _width(size), _height(size) {

  _blob.resize(_width * _height * TextureColor::RGBA_SZ);
  _blob.shrink_to_fit();
  for (size_t i = 0; i != _blob.size(); i = i + TextureColor::RGBA_SZ) {
    _blob[i] = std::byte(color.rgba[0]);
    _blob[i + 1] = std::byte(color.rgba[1]);
    _blob[i + 2] = std::byte(color.rgba[2]);
    _blob[i + 3] = std::byte(color.rgba[3]);
  }

  initTexture();
}

OsmTileTexture::OsmTileTexture(int size, const std::vector<std::byte> &blob) {
  _width = size;
  _height = size;
  _blob.resize(0);
  _blob.shrink_to_fit();

  int tmp;
  stbi_set_flip_vertically_on_load(false);
  const auto ptr = stbi_load_from_memory(
      reinterpret_cast<stbi_uc const *>(blob.data()),
      static_cast<int>(blob.size()), &_width, &_height, &tmp, STBI_rgb_alpha);
  _stbBlob.reset(ptr);

  initTexture();
}

OsmTileTexture::~OsmTileTexture() { glDeleteTextures(1, &_id); }

void OsmTileTexture::initTexture() {
  const auto ptr =
      _blob.size() > 0 ? (uint8_t *)_blob.data() : (uint8_t *)_stbBlob.get();

  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, ptr);
}
