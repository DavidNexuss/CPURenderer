#include <player.hpp>
#include <renderContext.hpp>
#include <thread>
using namespace std;

int main() {

  int width = 1920 / 2;
  int height = 1080 / 2;
  // Create screen buffer
  std::vector<color> screenBuffer(width * height);

  // Create player
  Player player(width, height, 3);

  // Create render context (CPU rendering utilities)
  RenderContext ctx;
  ctx.scr = Screen{width, height, screenBuffer.data()};
  ctx.brushColor = {255, 255, 255};

  // Render loop
  while (!player.shouldClose()) {
    // Clear screen buffer
    ctx.clear();
    // Printf in memory position
    ctx.printf(20, height - 35,
               " Esta pantalla es de %dx%d:\n  Este es un mensaje de prueba.",
               width, height);

    // Draw box
    ctx.drawBox(10, height - 50, 250, 40, 10);

    // Upload screen buffer to the GPU
    player.uploadFrame((char *)screenBuffer.data());
    player.drawFrame();

    // Sleep some time
    std::this_thread::sleep_for(16ms);
  }
}
