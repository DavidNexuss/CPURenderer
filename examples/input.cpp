#include "imgui.h"
#include <future>
#include <player.hpp>
#include <renderContext.hpp>
#include <thread>
using namespace std;

std::string text;

void testInput(RenderPlayer &player) {
  RenderContext ctx(player);

  color brushColor[] = {{255, 100, 100}, {100, 255, 100}, {100, 255, 255}};

  int top =
      std::min((player.getScreenHeight() - 10) / 10, GLFW_KEY_Z - GLFW_KEY_A) +
      GLFW_KEY_A;

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
  ctx.writeStr(150, player.getScreenHeight() - 20, text.c_str());

  player.uploadFrame();
  player.drawFrame(true);
  player.beginImgui();

  if (ImGui::Begin("Some window")) {
    ImGui::End();
  }
  player.endImgui();
  player.swapBuffers();
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
