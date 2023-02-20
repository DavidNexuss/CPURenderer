#include "player.hpp"
#include "screen.hpp"
#include <vector>
using namespace std;
void demo1(Screen &screen, PlayerConfiguration &configuration) {

  while (true) {
    int i = 0;
    int n = screen.len();
    int n2 = configuration.textureWidth;

    // Create sand
    for (int i = 0; i < n; i++) {
      int y =
          60 * i / (configuration.textureHeight * configuration.textureHeight);

      if ((rand() % 1000) < 1) {
        y += rand() % 15;
        screen.set(i, {u(40 + y * 2), u(30 + y), u(10 + y)});
      }
    }

    for (int y = 0; y < configuration.textureWidth; y++) {
      for (int fs = 0; fs < 1; fs++) {
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

void demo2(Screen &screen, PlayerConfiguration &configuration) {
  Context ctx(screen);

  for (int i = 0; i < 20; i++) {
    int x = 100 + rand() % (configuration.textureHeight - 200);
    int y = 100 + rand() % (configuration.textureWidth - 200);

    for (int j = 0; j < 100; j++) {
      screen.set(x, y + j, {255, 80, 80});
      screen.set(x + j, y, {255, 80, 80});

      screen.set(x, y - j, {255, 80, 80});
      screen.set(x - j, y, {255, 80, 80});
    }
  }
  vector<i2> pending;
  vector<i2> bpending;

  pending.push_back({20, 20});

  int it = 10000;
  while (it--) {

    // Extra
    if (0) {
      for (int i = 0; i < 1; i++) {
        int x = 100 + rand() % (configuration.textureHeight - 200);
        int y = 100 + rand() % (configuration.textureWidth - 200);

        for (int j = 0; j < 100; j++) {
          screen.set(x, y + j, {255, 80, 80});
          screen.set(x + j, y, {255, 80, 80});

          screen.set(x, y - j, {255, 80, 80});
          screen.set(x - j, y, {255, 80, 80});
        }
      }

      for (int i = 0; i < 250; i++) {
        int x = 100 + rand() % (configuration.textureHeight - 200);
        int y = 100 + rand() % (configuration.textureWidth - 200);
        color c = screen.get(x, y);
        if (c[0] == 255 && c[2] == 255) {
          screen.set(x, y, {1, 255, 255});
          bpending.push_back({x, y});

        } else if (c[0] == 255) {
          screen.set(x, y, {0, 0, 0});
        }
      }
    }
    for (i2 c : pending) {
      for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
          if (x == 0 && y == 0)
            continue;
          i2 d = c;
          d.x += x;
          d.y += y;
          if (d.x < 0 || d.y < 0 || d.x >= configuration.textureHeight ||
              d.y >= configuration.textureWidth)
            continue;

          if (screen.get(d.x, d.y)[0] == 0) {
            screen.set(d.x, d.y, {1, 255, 255});
            bpending.push_back(d);
          } else if (screen.get(d.x, d.y)[0] == 255) {
            screen.set(d.x, d.y, {255, 255, 255});
          }
        }
      }

      screen.set(c.x, c.y, {1, 80, 80});
    }
    pending.clear();
    pending.swap(bpending);
    screen.commit();
  }
}

int main() {
  PlayerConfiguration configuration;
  configuration.textureWidth = 640;
  configuration.textureHeight = 480;
  configuration.textureChannels = 3;
  Screen screen(configuration);

  demo2(screen, configuration);
}
