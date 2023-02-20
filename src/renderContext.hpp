#pragma once
#include "screen.hpp"
#include <chrono>
#include <cstdio>
#include <vector>

class RenderContext {

  Screen scr;
  color brushColor;
  std::chrono::milliseconds writeCharDelay;

public:
  RenderContext() {}
  RenderContext(Screen _scr) : scr(_scr) {}

  inline void setBrushColor(color col) { brushColor = col; }
  inline void setWriteCharDelay(std::chrono::milliseconds delay) {
    writeCharDelay = delay;
  }

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
