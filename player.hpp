#pragma once
#include <memory>
#include <thread>
#include <vector>

struct PlayerConfiguration {
  int textureWidth;
  int textureHeight;
  int textureChannels;
};

struct PlayerInternal;

class Player {
  std::shared_ptr<PlayerInternal> internal;
  std::thread renderThread;

public:
  Player();

  void drawFrame();
  void uploadFrame(char *data, int width, int height, int ch);
  bool shouldClose();

  char *launch(int width, int height, int ch);
};
