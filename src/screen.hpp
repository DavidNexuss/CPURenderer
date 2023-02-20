#pragma once
#include <array>
using color = std::array<unsigned char, 3>;
using u = unsigned char;

class Screen {

  struct ScreenProxy {
    color *scr;
    int x;
    int width;

  public:
    inline color &operator[](int y) { return scr[y * width + x]; }
  };

protected:
  int width;
  int height;
  color *scr;

public:
  inline ScreenProxy operator[](int x) { return {scr, x, width}; }
  inline color *native() { return scr; }
  inline int count() { return width * height; }
};

template <typename T> color Color(T a, T b, T c) { return {u(a), u(b), u(c)}; }
static color mul(color a, color b) {
  return Color(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}
