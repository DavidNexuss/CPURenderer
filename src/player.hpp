#pragma once
#include <memory>
#include <thread>
#include <vector>

struct PlayerInternal;

class Player {
  std::shared_ptr<PlayerInternal> internal;
  std::thread renderThread;

  int width;
  int height;
  int ch;

public:
  Player(int width, int height, int ch);

  void drawFrame();
  void uploadFrame(char *scr);
  bool shouldClose();

  void launch(char **scr);
};
