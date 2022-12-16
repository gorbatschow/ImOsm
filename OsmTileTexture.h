#pragma once
#include <GL/gl.h>
#include <array>
#include <imgui.h>
#include <memory.h>
#include <memory>
#include <stb_image.h>
#include <vector>

struct TextureColor {
  static constexpr int RGBA_SZ{4};
  static constexpr std::array<uint8_t, RGBA_SZ> Snow{255, 250, 250, 255};
  static constexpr std::array<uint8_t, RGBA_SZ> Gold{255, 215, 0, 255};
  static constexpr std::array<uint8_t, RGBA_SZ> Aqua{0, 255, 255, 255};
  static constexpr std::array<uint8_t, RGBA_SZ> Lime{0, 255, 0, 255};
  static constexpr std::array<uint8_t, RGBA_SZ> Slate{112, 128, 144, 255};
  static constexpr std::array<uint8_t, RGBA_SZ> Coral{255, 127, 80, 255};

  TextureColor() : rgba(Snow) {}
  TextureColor(const std::array<uint8_t, RGBA_SZ> &color) : rgba{color} {};

  const std::array<uint8_t, RGBA_SZ> rgba{Snow};
};

class OsmTileTexture {
public:
  OsmTileTexture(int size = 256, TextureColor color = TextureColor::Snow);
  OsmTileTexture(int size, const std::vector<std::byte> &blob);
  ~OsmTileTexture();

  inline GLuint glID() const { return _id; }
  inline ImTextureID imID() const { return (ImTextureID)(intptr_t)_id; }

private:
  GLuint _id{};
  int _width{256}, _height{256}, _channels{};
  std::vector<std::byte> _blob;

  void initTexture();
};
