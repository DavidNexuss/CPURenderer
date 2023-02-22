#include <chrono>
#include <player.hpp>
#include <thread>
#include <vector>
using namespace std;

int main() {
  int width = 1920 / 2;
  int height = 1920 / 2;

  Player pa(width, height);
  Player pb(width, height);
  RGBScreenBuffer scra(width, height);
  RGBScreenBuffer scrb(width, height);

  auto athread = pa.launch(scra);
  auto bthread = pb.launch(scrb);

  int i = 0;
  while (!pa.shouldClose()) {

    for (int fs = 0; fs < 20; fs++) {
      scra.native()[i] = {i / width, i % width, i % (width)};
      scrb.native()[i] = {i % width, i / width, i % (width)};
      i++;
      i %= scrb.count();
    }

    std::this_thread::sleep_for(20us);
  }
}
