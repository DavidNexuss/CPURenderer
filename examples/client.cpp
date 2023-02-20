#include "debug.hpp"
#include "player.hpp"
#include "util.hpp"
#include <unistd.h>
#include <vector>

using namespace std;

int main() {

  LOG("-> Awaiting input...\n");
  PlayerConfiguration configuration;
  read(0, &configuration, sizeof(configuration));
  Player player(configuration);
  vector<unsigned char> frame(configuration.width * configuration.height *
                              configuration.channels);

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
        player.uploadFrame((char *)frame.data());
      }
    }
    player.drawFrame();
  }
}
