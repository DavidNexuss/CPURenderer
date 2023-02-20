#pragma once
#include "screen.hpp"
#include <cstdio>

class RenderContext {
public:
  Screen scr;
  color brushColor;

  void clear();
  void writeChar(int ch, int x, int y);
  void drawBox(int x, int y, int width, int height, int c);
  void fillBox(int x, int y, int width, int height);

  void writeStr(int x, int y, const char *);

  template <typename... Args> void printf(int x, int y, Args &&...args) {
    static std::vector<char> buffer(4096);

    int bufferSize = sprintf(buffer.data(), std::forward<Args>(args)...);
    int originalX = x;

    writeStr(x, y, buffer.data());
  }
};
