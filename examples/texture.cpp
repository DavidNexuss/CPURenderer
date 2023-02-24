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

  Player player(width, height);
  ScreenBuffer<unsigned char, 3> scr(width, height);
  ScreenBuffer<unsigned char, 3> overlay(width, height);

  RenderContext ctx(scr);
  ctx.setBrushColor({0, 0, 0});
  RenderContext ctxo(overlay);

  ctx.writeTexture(20, 20, "../assets/sample.png");

  auto t = std::thread([&]() {
    while (true) {
      int x = (mrand() % (scr.width - 2)) + 1;
      int y = (mrand() % (scr.height - 2)) + 1;

      int px = (mrand() % 3) - 1;
      int py = (mrand() % 3) - 1;

      std::swap(scr[x][y], scr[x + px][y + py]);
    }
  });

  ctx.writeStr(20, height - 20,
               "Este es un ejemplo de la carga de texturas y del overlay");
  player.uploadTexture(overlay, 1);

  while (!player.shouldClose()) {
    if (player.isKeyJustPressed('E')) {
      ctx.writeTexture(20, 20, "../assets/sample.png");
    }
    player.clear();
    player.uploadTexture(scr);
    player.drawTexture(0);
    player.drawTexture(1);
    player.swapBuffers();
    std::this_thread::sleep_for(16ms);
  }
}
