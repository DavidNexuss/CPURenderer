#include "debug.hpp"
#include "player.hpp"
#include "util.hpp"
#include <unistd.h>
#include <vector>

using namespace std;

struct PlayerConfiguration {
  int textureWidth;
  int textureHeight;
  int textureChannels;
};

int main() {

  Player player;
  LOG("-> Awaiting input...\n");
  PlayerConfiguration configuration;
  read(0, &configuration, sizeof(configuration));

  vector<unsigned char> frame(configuration.textureWidth *
                              configuration.textureHeight *
                              configuration.textureChannels);

  bool reachEnd = false;
  while (!player.shouldClose()) {

    if (!reachEnd) {
      int status = readall(0, frame.data(), frame.size());
      if (status == 0) {
        LOG("EOF\n");
        reachEnd = true;
      } else if (status != frame.size()) {
        LOG("Bad frame\n");
        break;
      } else {
        player.uploadFrame((char *)frame.data(), configuration.textureWidth,
                           configuration.textureHeight,
                           configuration.textureChannels);
      }
    }
    player.drawFrame();
  }
}
