#pragma once
#include "screen.hpp"
#include <chrono>
#include <cstdio>
#include <vector>

class RenderContext {

  RGBScreen scr;
  rgb brushColor{255, 255, 255};
  std::chrono::milliseconds writeCharDelay = std::chrono::milliseconds::zero();

public:
  RenderContext() {}
  RenderContext(RGBScreen _scr) : scr(_scr) {}

  /**
   * Sets the active brushColor everything is rendered using brushColor
   */
  inline void setBrushColor(const rgb& col) { brushColor = col; }

  /**
   * Sets a delay between each writeChar invokation, useful for animations
   */
  inline void setWriteCharDelay(std::chrono::milliseconds delay) {
    writeCharDelay = delay;
  }

  /*
   * Clears screen
   */
  void clear() const;

  /*
   * Writes char at a given position
   */
  void writeChar(int ch, int x, int y) const;

  /*
   * Draws box in a given position with a border size
   */
  void drawBox(int x, int y, int width, int height, int borderSize) const;

  /*
   * Draws a filled box
   */
  void fillBox(int x, int y, int width, int height) const;

  /*
   * Writes a string at a given position
   */
  void writeStr(int x, int y, const char *) const;

  /*
   * Performs a from the source buffer to the screen buffer
   */
  void writeData(int x, int y, int width, int height, unsigned char *data);

  /**
   * Writes a texture to the given position
   */
  void writeTexture(int x, int y, const char *texturePath);

  /*
   * printf equivalent with writeStr
   */
  template <typename... Args> void printf(int x, int y, Args &&...args) const {
    static std::vector<char> buffer(4096);
    sprintf(buffer.data(), std::forward<Args>(args)...);
    writeStr(x, y, buffer.data());
  }
};
