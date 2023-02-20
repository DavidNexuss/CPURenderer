#pragma once
#include <array>
using color = std::array<unsigned char, 3>;
using u = unsigned char;

class Screen {

  inline static color crap;
  struct ScreenProxy {
    color *scr;
    int x;
    int width;
    int top;

  public:
    inline color &operator[](int y) const {
      int index = y * width + x;
      if (index < 0 || index >= top)
        return crap;
      return scr[y * width + x];
    }
  };

protected:
  int width;
  int height;
  color *scr;

public:
  inline ScreenProxy operator[](int x) const {
    return {scr, x, width, width * height};
  }
  inline color *native() const { return scr; }
  inline int count() const { return width * height; }

  inline int getScreenWidth() const { return width; }
  inline int getScreenHeight() const { return height; }

  Screen(int width, int height, color *_scr) {
    this->width = width;
    this->height = height;
    this->scr = _scr;
  }

  Screen() {}
};

template <typename T> color Color(T a, T b, T c) { return {u(a), u(b), u(c)}; }
static color mul(color a, color b) {
  return Color(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

#include <vector>
struct ScreenBuffer : public Screen {
  std::vector<color> buffer;
  ScreenBuffer(int width, int height) : buffer(width * height) {
    Screen::width = width;
    Screen::height = height;
    Screen::scr = buffer.data();
  }

  ScreenBuffer(const ScreenBuffer &other) {
    buffer = other.buffer;
    Screen::scr = buffer.data();
    Screen::width = other.width;
    Screen::height = other.height;
  }
};
