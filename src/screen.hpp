#pragma once
#include "player.hpp"
#include <array>
using color = std::array<unsigned char, 3>;
using u = unsigned char;

class Screen {
  Player player;
  color *scr;
  int width;

  struct ScreenProxy {
    color *scr;
    int x;
    int width;

  public:
    inline color &operator[](int y) { return scr[y * width + x]; }
  };

public:
  Screen(int width, int height) {
    scr = (color *)player.launch(width, height, 3);
    this->width = width;
  }

  Screen(Screen &) = delete;
  ScreenProxy operator[](int x) { return {scr, x, width}; }

  inline color *native() { return scr; }
};
