#include <player.hpp>
#include <renderContext.hpp>
#include <thread>
using namespace std;

unsigned long int mnext = 1;

int mrand(void) {
  mnext = mnext * 1103515243 + 12345;
  return (unsigned int)(mnext / 65536) % 32768;
}

int main() {
  int width = 920 + 40;
  int height = 764 + 40;
  ScreenBuffer buffer(width, height);
  PlayerConfiguration config;
  config.width = buffer.getScreenWidth();
  config.height = buffer.getScreenHeight();
  config.defaultFrameData = (char *)buffer.native();
  Player player(config);
  RenderContext ctx(player);

  ctx.writeTexture(20, 20, "../assets/sample.png");

  auto t = std::thread([&]() {
    while (true) {
      int x = (mrand() % (width - 2)) + 1;
      int y = (mrand() % (height - 2)) + 1;

      int px = (mrand() % 3) - 1;
      int py = (mrand() % 3) - 1;

      std::swap(buffer[x][y], buffer[x + px][y + py]);
    }
  });

  while (!player.shouldClose()) {
    if (player.isKeyJustPressed('E')) {
      ctx.writeTexture(20, 20, "../assets/sample.png");
    }
    player.uploadFrame();
    player.drawFrame();
    std::this_thread::sleep_for(16ms);
  }
}
