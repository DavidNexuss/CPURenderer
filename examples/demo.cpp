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
  PlayerConfiguration configuration;
  configuration.width = width;
  configuration.height = height;
  configuration.defaultFrameData = (char *)screenBuffer.data();

  Player player(configuration);

  // Create render context (CPU rendering utilities)
  RenderContext ctx(player);
  ctx.setBrushColor({255, 255, 255});

  // Render loop
  int i = 0;
  while (!player.shouldClose()) {
    // Printf in memory position
    ctx.printf(20, height - 35,
               " Esta pantalla es de %dx%d:\n  Este es un mensaje de prueba.",
               width, height);

    // Draw box
    ctx.drawBox(10, height - 50, 250, 40, 10);

    // Write a white pixel into coordinates (20,100)
    player[20][100] = {255, 255, 255};

    // Loop and write interpolation
    for (int fs = 0; fs < 800; fs++) {
      screenBuffer[i] = {u(i / width), u(i % width), u(i % (width / 2))};
      i++;
      i %= player.count();
    }
    // Upload screen buffer to the GPU
    player.uploadFrame();
    player.drawFrame();

    // Sleep some time
    std::this_thread::sleep_for(20ms);
  }
}
