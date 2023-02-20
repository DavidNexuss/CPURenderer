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
  RenderPlayer player(920 + 40, 764 + 40);
  RenderContext ctx(player);

  ctx.writeTexture(20, 20, "../assets/sample.png");

  auto t = std::thread([&]() {
    while (true) {
      int x = (mrand() % (player.getScreenWidth() - 2)) + 1;
      int y = (mrand() % (player.getScreenHeight() - 2)) + 1;

      int px = (mrand() % 3) - 1;
      int py = (mrand() % 3) - 1;

      std::swap(player[x][y], player[x + px][y + py]);
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
