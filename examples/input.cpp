#include "imgui.h"
#include <future>
#include <player.hpp>
#include <renderContext.hpp>
#include <thread>
#include <array>
using namespace std;

std::string text;

void testInput(Player& player, RGBScreen& scr) {
  RenderContext ctx(scr);

  std::array<rgb, 3> brushColor = {rgb(255, 100, 100), rgb(100, 255, 100), rgb(100, 255, 255)};

  int top =
      std::min((scr.width - 10) / 10, GLFW_KEY_Z - GLFW_KEY_A) + GLFW_KEY_A;

  ctx.clear();
  for (int i = GLFW_KEY_A; i <= top; i++) {
    int index = player.isKeyPressed(i) + player.isKeyJustPressed(i);
    ctx.setBrushColor(brushColor[index]);
    ctx.printf(10, 10 + 10 * (i - GLFW_KEY_A), "Tecla %c tecleada", char(i));
    if (index == 2) {
      text += char(i);
    }
  }

  ctx.setBrushColor({255, 255, 255});
  ctx.printf(200, 100, "cursor: %d %d", player.getX(), player.getY());
  ctx.printf(200, 110, "ventana: %d %d", player.getWindowWidth(),
             player.getWindowHeight());
  ctx.writeStr(150, scr.height - 20, text.c_str());

  player.uploadTexture(scr);
  player.drawTexture();
  player.beginImgui();

  if (ImGui::Begin("Some window")) {
    ImGui::End();
  }
  player.endImgui();
  player.swapBuffers();
}

int main() {
  int width = 1920 / 2;
  int height = 1080 / 2;
  Player pa(width, height);
  Player pb(width, height);

  RGBScreenBuffer scra(width, height);
  RGBScreenBuffer scrb(width, height);

  while (!pa.shouldClose() && !pb.shouldClose()) {
    testInput(pa,scra);
    testInput(pb, scrb);

    std::this_thread::sleep_for(10ms);
  }
}
