#include <chrono>
#include <player.hpp>
#include <thread>
#include <vector>
using namespace std;

struct ScreenBuffer : public Screen {
  std::vector<color> buffer;
  ScreenBuffer(int width, int height) : buffer(width * height) {
    Screen::width = width;
    Screen::height = height;
    Screen::scr = buffer.data();
  }
};

int main() {
  int width = 1920 / 2;
  int height = 1920 / 2;

  ScreenBuffer screenA(width, height);
  ScreenBuffer screenB(width, height);

  PlayerConfiguration configuration;
  configuration.width = screenA.getScreenWidth();
  configuration.height = screenA.getScreenHeight();
  configuration.defaultFrameData = (char *)screenA.native();

  Player a(configuration);
  configuration.defaultFrameData = (char *)screenB.native();
  Player b(configuration);

  a.launch();
  b.launch();

  int i = 0;
  while (!a.shouldClose()) {

    for (int fs = 0; fs < 20; fs++) {
      screenA.native()[i] = {u(i / width), u(i % width), u(i % (width))};
      screenB.native()[i] = {u(i % width), u(i / width), u(i % (width))};
      i++;
      i %= screenA.count();
    }

    std::this_thread::sleep_for(20us);
  }
}
