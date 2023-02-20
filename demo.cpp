#include "player.hpp"
#include <array>

#include <chrono>
#include <cstdio>
#include <glm/glm.hpp>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

using color = std::array<unsigned char, 3>;
using u = unsigned char;

color *scr;
int scrn;

void draw() {}
int main() {

  std::cout << (1290ms + 1s).count() << std::endl;
  Player player;
  int width = 640;
  int height = 480;
  scr = (color *)player.launch(width, height, 3);

  int i = 0;
  while (1) {
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        scr[y * width + x] = {u(i), u(x), u(y)};
        std::this_thread::sleep_for(20ns);
      }
    }
    i++;
  }
}
