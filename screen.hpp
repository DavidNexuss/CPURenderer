#include "debug.hpp"
#include "player.hpp"
#include "util.hpp"
#include <array>
#include <iostream>
#include <unistd.h>
#include <vector>

using u = unsigned char;
using color = std::array<u, 3>;
class Screen {
  PlayerConfiguration configuration;
  std::vector<color> data;
  bool first = true;

public:
  Screen(PlayerConfiguration _configuration)
      : configuration(_configuration),
        data(_configuration.textureWidth * _configuration.textureHeight) {}

  void clear() {
    for (int i = 0; i < data.size(); i++) {
      data[i] = {0, 0, 0};
    }
  }

  void commit() {
    if (first) {
      writeall(1, &configuration, sizeof(configuration));
      first = false;
    }
    writeall(1, data.data(), data.size() * sizeof(color));
  }
  void set(int x, int y, color c) {
    data[x * configuration.textureWidth + y] = c;
  }

  color get(int x, int y) { return data[x * configuration.textureWidth + y]; }

  void set(int i, color c) { data[i] = c; }
  int len() { return data.size(); }

  int getWidth() { return configuration.textureWidth; }
  int getHeight() { return configuration.textureHeight; }
};

struct f2 {
  float x;
  float y;
};

struct f3 : public f2 {
  float z;
};

struct f4 : public f3 {
  float w;
};

struct i2 {
  int x, y;
};

class Context {
  Screen &screen;
  color brushColor;

public:
  Context(Screen &_screen) : screen(_screen) {}

  void setBrushColor(color _brush) { this->brushColor = _brush; }
  void pixel(f2 p) {
    i2 t = transform(p);
    screen.set(t.x, t.y, brushColor);
  }
  void line(f2 start, f2 end) {
    if (end.x < start.x || end.y < start.y) {
      std::swap(start, end);
    }
    i2 istart = transform(start);
    i2 iend = transform(end);
    int x = istart.x;
    int x2 = iend.x;
    int y = istart.y;
    int y2 = iend.y;

    LOG("%d %d %d %d\n", x, y, x2, y2);
    int dx, dy, p;
    dx = x2 - x;
    dy = y2 - y;
    p = 2 * (dy) - (dx);
    while (x <= x2) {
      if (p < 0) {
        x = x + 1;
        y = y;
        p = p + 2 * (dy);
      } else {
        x = x + 1;
        y = y + 1;
        p = p + 2 * (dy - dx);
      }
      screen.set(x, y, brushColor);
    }
  }

  i2 transform(f2 p) {
    return {int((p.x * 0.5f + 0.5f) * screen.getHeight()),
            int((p.y * 0.5f + 0.5f) * screen.getWidth())};
  }
};

#include <glm/glm.hpp>
