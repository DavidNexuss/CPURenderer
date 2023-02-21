#include <cmath>
#include <imgui.h>
#include <player.hpp>
#include <renderContext.hpp>
#include <vector>
struct ivec {
  int x;
  int y;
};

void rasterLine(Player &player, ivec start, ivec end) {
  ivec difference = {end.x - start.x, end.y - start.y};
  if (difference.x == 0)
    return;

  float l =
      std::sqrt(difference.x * difference.x + difference.y * difference.y);

  float dx = difference.x / l;
  float dy = difference.y / l;

  float rx = start.x;
  float ry = start.y;

  for (float i = 0; i < l; i += 0.1) {
    rx += dx * 0.1;
    ry += dy * 0.1;
    player[int(rx)][int(ry)] = {255, 255, 255};
  }
}

int main() {
  RenderPlayer player(1920 / 2, 1080 / 2);
  RenderContext ctx(player);
  int squareIndex = 0;
  ivec square[4];

  while (!player.shouldClose()) {
    ctx.printf(20, player.getScreenHeight() - 20,
               "Posicion del cursor: %f %f, square %d", (float)player.getX(),
               (float)player.getY(), squareIndex);

    int x = player.getX() * player.getScreenWidth();
    int y = player.getY() * player.getScreenHeight();

    for (int i = 0; i < 4; i++) {
      rasterLine(player, square[i], square[(i + 1) % 4]);
    }

    if (player.isKeyPressed(GLFW_MOUSE_BUTTON_LEFT)) {
      player[x][y] = {255, 255, 255};
    }

    player[x][y] = {255, 255, 255};
    player.uploadFrame();
    player.drawFrame(true);
    player.beginImgui();
    if (ImGui::Begin("Some window")) {
      ImGui::End();
    }
    player.endImgui();
    player.swapBuffers();
  }
}
