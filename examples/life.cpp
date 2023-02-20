#include "util.hpp"
#include <iostream>
#include <player.hpp>
#include <renderContext.hpp>

int main() {
  int width = 1920;
  int height = 1080;
  ScreenBuffer screen(width, height);
  ScreenBuffer backbuffer(width, height);

  PlayerConfiguration configuration;
  configuration.width = width;
  configuration.height = height;
  string fs = readFile("../assets/glow.frag");
  configuration.fragmentShaderOverride = fs.c_str();

  Player player(configuration);

  int li = 10;
  int up = 0;

  RenderContext ctx(backbuffer);
setup:
  for (int i = 0; i < screen.count(); i++) {
    if ((rand() % 100) < 2) {
      screen.native()[i] = {255, 255, 255};
      backbuffer[i] = screen[i];
    } else {
      screen.native()[i] = {0, 0, 0};
    }
    backbuffer.native()[i] = screen.native()[i];
  }

  int Salive = 3;
  int SdeadUp = 3;
  int SdeadDown = 2;

  while (!player.shouldClose()) {
    if (player.isKeyJustPressed('R')) {
      goto setup;
    }
    Salive += player.isKeyJustPressed('Q') - player.isKeyJustPressed('A');
    SdeadDown += player.isKeyJustPressed('W') - player.isKeyJustPressed('S');
    SdeadUp += player.isKeyJustPressed('E') - player.isKeyJustPressed('D');
    for (int i = 1; i < (width - 1); i++) {
      for (int j = 1; j < (height - 1); j++) {
        int alive = 0;
        for (int x = -1; x <= 1; x++) {
          for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == x)
              continue;
            int px = x + i;
            int py = y + j;
            if (screen[px][py][0] > 0)
              alive++;
          }
        }
        if (screen[i][j][0] > 0) {
          if (alive < SdeadDown || alive > SdeadUp) {
            backbuffer[i][j] = {0, 0, 0};
          }
        } else {
          if (alive == Salive) {
            backbuffer[i][j] = {u(li * 2), u(li * 3), u(li * 5)};
          }
        }
      }
    }

    ctx.printf(20, player.getScreenHeight() - 20, "%d %d %d", Salive, SdeadDown,
               SdeadUp);
    ctx.printf(20, player.getScreenWidth() - 40,
               "Use keys Q-A W-S E-D for changing life rules");
    screen = backbuffer;
    player.uploadFrame((char *)screen.native());
    player.drawFrame();
    up++;
    if (up > 5) {
      li++;
      up = 0;
    }
  }
}
