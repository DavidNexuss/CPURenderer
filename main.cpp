#include "player.hpp"

int main() {
  PlayerConfiguration configuration;
  configuration.textureWidth = 400;
  configuration.textureHeight = 400;
  configuration.textureChannels = 3;
  Screen screen(configuration);
  Context ctx(screen);

  int i = 0;
  int n = screen.len();
  int n2 = configuration.textureWidth;

  // Create sand
  for (int i = 0; i < n; i++) {
    int y =
        60 * i / (configuration.textureHeight * configuration.textureHeight);

    if ((rand() % 100) < 60) {
      y += rand() % 15;
      screen.set(i, {u(40 + y * 2), u(30 + y), u(10 + y)});
    }
  }

  while (true) {
// Apply gravity
//
//
#pragma omp parallel for
    for (int y = 0; y < configuration.textureWidth; y++) {
      for (int fs = 0; fs < 10; fs++) {
        for (int x = 0; x < configuration.textureHeight - 1; x++) {
          color target = screen.get(x + 1, y);
          color current = screen.get(x, y);

          if (current[0] == 0 && target[0] != 0) {
            screen.set(x, y, target);
            screen.set(x + 1, y, {0, 0, 0});
          }
        }
      }
    }
    screen.commit();
  }
}
