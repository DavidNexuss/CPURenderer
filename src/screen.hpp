#pragma once
#include "color.hpp"
#include "texture.hpp"
#include <vector>

template <typename B, int channelCount> class Screen : public TextureFormat {

public:
  using Texel = vec<B, channelCount>;

private:
  struct ScreenProxy {
    Texel *scr;
    int x;
    int width;

  public:
    Texel &operator[](int y) const { return scr[y * width + x]; }
  };

public:
  ScreenProxy operator[](int x) const { return {(Texel *)source, x, width}; }

  Texel *native() const { return (Texel *)source; }
  int count() const { return width * height; }

  Screen(int width, int height, Texel *_scr) {
    this->width = width;
    this->height = height;
    this->scr = _scr;
  }

  Screen() {}
};

template <typename B, int channelCount>
struct ScreenBuffer : public Screen<B, channelCount> {
  std::vector<typename Screen<B, channelCount>::Texel> buffer;

  ScreenBuffer(int width, int height) : buffer(width * height) {
    this->width = width;
    this->height = height;
    this->source = buffer.data();
  }

  ScreenBuffer(const ScreenBuffer &other) {
    buffer = other.buffer;
    this->width = other.width;
    this->height = other.height;
    this->source = buffer.data();
  }
};
using RGBScreen = Screen<unsigned char, 3>;
using RGBScreenBuffer = ScreenBuffer<unsigned char, 3>;
