#include "debug.hpp"
#include "player.hpp"
#include "util.hpp"
#include <unistd.h>
#include <vector>

using namespace std;

int main() {

  LOG("-> Awaiting input...\n");
  TextureFormat format;
  read(0, &format, sizeof(format));

  Player player(format.width, format.height);
  RGBScreenBuffer scr(format.width, format.height);
  bool reachEnd = false;

  /*
    while (!player.shouldClose()) {

      if (!reachEnd) {
        int status = readall(0, .data(), frame.size());
        if (status == 0) {
          LOG("EOF\n");
          reachEnd = true;
        } else if (status != frame.size()) {
          LOG("Bad frame\n");
          break;
        } else {
          player.uploadFrame((char *)frame.data());
        }
      }
      player.drawFrame();
    } */
}
