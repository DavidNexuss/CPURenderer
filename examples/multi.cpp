#include <chrono>
#include <player.hpp>
#include <thread>
#include <vector>
using namespace std;

int main() {
  int width = 1920 / 2;
  int height = 1920 / 2;

  RenderPlayer a(width, height);
  RenderPlayer b(width, height);

  a.launch();
  b.launch();

  int i = 0;
  while (!a.shouldClose()) {

    for (int fs = 0; fs < 20; fs++) {
      a.native()[i] = {u(i / width), u(i % width), u(i % (width))};
      b.native()[i] = {u(i % width), u(i / width), u(i % (width))};
      i++;
      i %= b.count();
    }

    std::this_thread::sleep_for(20us);
  }
}
