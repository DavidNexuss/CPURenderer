#pragma once
#include "screen.hpp"
#include <cstdio>

class RenderContext {
  Screen scr;
  int count;

public:
  int width;
  int height;

  RenderContext(int width, int height);
  inline Screen &screen() { return scr; }

  void clear();

  void writeChar(int ch, int x, int y, color c);
  void drawBox(int x, int y, int width, int height, int c);

  void writeStr(color c, int x, int y, const char *);

  template <typename... Args>
  void printf(color c, int x, int y, Args &&...args) {
    static std::vector<char> buffer(4096);

    int bufferSize = sprintf(buffer.data(), std::forward<Args>(args)...);
    int originalX = x;

    writeStr(c, x, y, buffer.data());
  }
};
