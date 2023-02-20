#include "util.hpp"
#include <iostream>
#include <player.hpp>

int main() {
  int n = 1000;
  std::vector<color> screen(n * n);

  PlayerConfiguration configuration;
  configuration.width = n;
  configuration.height = n;
  string fs = readFile("../assets/glow.frag");
  configuration.fragmentShaderOverride = fs.c_str();

  Player player(configuration);

  int li = 10;
  int up = 0;

setup:
  for (int i = 0; i < screen.size(); i++) {
    if ((rand() % 100) < 2) {
      screen[i] = {255, 255, 255};
    } else {
      screen[i] = {0, 0, 0};
    }
  }

  std::vector<color> backbuffer = screen;
  while (!player.shouldClose()) {
    if (player.isKeyJustPressed('R')) {
      goto setup;
    }
    for (int i = 1; i < (n - 1); i++) {
      for (int j = 1; j < (n - 1); j++) {
        int alive = 0;
        for (int x = -1; x <= 1; x++) {
          for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == x)
              continue;
            int px = x + i;
            int py = y + j;
            if (screen[px * n + py][0] > 0)
              alive++;
          }
        }
        int index = i * n + j;
        if (screen[index][0] > 0) {
          if (alive < 2 || alive > 4) {
            screen[index] = {0, 0, 0};
          }
        } else {
          if (alive == 3) {
            screen[index] = {u(li * 2), u(li * 3), u(li * 5)};
          }
        }
      }
    }

    player.uploadFrame((char *)screen.data());
    player.drawFrame();
    screen.swap(backbuffer);
    up++;
    if (up > 5) {
      li++;
      up = 0;
    }
  }
}
