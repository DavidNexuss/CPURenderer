#include <player.hpp>
#include <renderContext.hpp>
#include <thread>
using namespace std;

int main() {

  int width = 1920 / 2;
  int height = 1080 / 2;

  RenderContext ctx(width, height);
  color white = {255, 255, 255};

  int counter = 80;

  ctx.printf({u(counter), u(counter * 2), u(counter * 3)}, 10, height - 10,
             " Esta pantalla es de %dx%d:\n  Este es un mensaje de "
             "prueba.\n Contador: %d",
             width, height, counter);

  std::this_thread::sleep_for(1000000s);
}
