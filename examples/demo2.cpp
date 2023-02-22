#include <fstream>
#include <future>
#include <iostream>
#include <istream>
#include <player.hpp>
#include <renderContext.hpp>
#include <sstream>
#include <thread>
#include <vector>
using namespace std;

string readFile(const char *path) {
  ifstream ifs;
  ifs.open(path);
  stringstream ss;
  ss << ifs.rdbuf();
  return ss.str();
}

#define A(X) std::async(std::launch::async, [=]() { X });
int main() {

  int width = 1920;
  int height = 1080;

  Player player(width, height);
  RGBScreenBuffer scr(width, height);
  auto renderThread = player.launch(scr);

  RenderContext ctx(scr);
  ctx.setBrushColor({255, 100, 100});
  ctx.setWriteCharDelay(5ms);

  RenderContext a = ctx;
  RenderContext b = ctx;
  b.setBrushColor({255, 100, 255});

  auto stop = A(a.writeStr(20, height - 20, readFile("../README.md").c_str()););
  std::this_thread::sleep_for(200ms);
  auto stop2 =
      A(b.writeStr(1000, height - 20, readFile("../CMakeLists.txt").c_str()););

  std::this_thread::sleep_for(20000ms);
}
