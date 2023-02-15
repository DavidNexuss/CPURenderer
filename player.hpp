#include "util.hpp"
#include <array>
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;
using color = std::array<unsigned char, 3>;
using u = unsigned char;

struct PlayerConfiguration {
  int textureWidth;
  int textureHeight;
  int textureChannels;
};

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
};

class Context {
  Screen &screen;
  color brushColor;

public:
  Context(Screen &_screen) : screen(_screen) {}

  void setBrushColor(color _brush) { this->brushColor = _brush; }
  void pixel(int x, int y) { screen.set(x, y, brushColor); }
  void line(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int d = 2 * dy - dx;
    int incr_e = 2 * dy;
    int incr_ne = 2 * (dy - dx);
    int x = x1;
    int y = y1;
    pixel(x, y);

    int j = 0;
    while (x < x2) {
      if (d <= 0) {
        d += incr_e;
        x++;
      } else {
        d += incr_ne;
        x++;
        y++;
      }
      pixel(x, y);
    }
  }
};

#include <glm/glm.hpp>
