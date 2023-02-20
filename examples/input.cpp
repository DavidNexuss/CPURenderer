#include <future>
#include <player.hpp>
#include <renderContext.hpp>
#include <thread>
using namespace std;

struct RenderPlayer : public ScreenBuffer, public Player {

  PlayerConfiguration configure() {
    PlayerConfiguration configuration;
    configuration.width = ScreenBuffer::getScreenWidth();
    configuration.height = ScreenBuffer::getScreenHeight();
    configuration.defaultFrameData = (char *)ScreenBuffer::native();
    return configuration;
  }
  RenderPlayer(int width, int height)
      : ScreenBuffer(width, height), Player(configure()) {}

  Screen toScreen() { return *(ScreenBuffer *)this; }
};

void testInput(RenderPlayer &player) {
  RenderContext ctx(player.toScreen());

  color brushColor[] = {{255, 100, 100}, {100, 255, 100}};

  int top = std::min((player.toScreen().getScreenHeight() - 10) / 10,
                     GLFW_KEY_Z - GLFW_KEY_A) +
            GLFW_KEY_A;

  ctx.clear();
  for (int i = GLFW_KEY_A; i <= top; i++) {
    ctx.setBrushColor(brushColor[player.isKeyPressed(i)]);
    ctx.printf(20, 10 + 10 * (i - GLFW_KEY_A), "Tecla %c tecleada", char(i));
  }

  ctx.setBrushColor({255, 255, 255});
  ctx.printf(200, 100, "Posicion del cursor: %d %d", player.getX(),
             player.getY());
  ctx.printf(200, 110, "Tamanyo de la ventana: %d %d", player.getWindowWidth(),
             player.getWindowHeight());

  player.uploadFrame();
  player.drawFrame();
}

int main() {
  RenderPlayer pa(1920 / 2, 1080 / 2);
  RenderPlayer pb(1920 / 2, 1080 / 2);

  while (!pa.shouldClose() && !pb.shouldClose()) {
    testInput(pa);
    testInput(pb);

    std::this_thread::sleep_for(10ms);
  }
}
