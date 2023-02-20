#include <player.hpp>
#include <renderContext.hpp>
#include <thread>
using namespace std;

int main() {
  int width = 1920;
  int height = 1080;
  ScreenBuffer buffer(width, height);
  PlayerConfiguration config;
  config.width = buffer.getScreenWidth();
  config.height = buffer.getScreenHeight();
  config.defaultFrameData = (char *)buffer.native();
  Player player(config);
  RenderContext ctx(player);

  ctx.writeTexture(20, 20, "../assets/sample.png");

  player.uploadFrame();

  while (!player.shouldClose()) {
    player.drawFrame();
    std::this_thread::sleep_for(50ms);
  }
}
