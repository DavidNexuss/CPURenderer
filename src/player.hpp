#pragma once
#include "screen.hpp"
#include <memory>
#include <thread>
#include <vector>

struct PlayerInternal;

struct PlayerConfiguration {

  /* Texture configuration */
  int width = 640;
  int height = 480;
  int channels = 3;
  bool isHDR = false;

  /* Shader content for overriding in player*/
  const char *fragmentShaderOverride = nullptr;
  const char *vertexShaderOverride = nullptr;

  const char *titleOverride = nullptr;

  char *defaultFrameData = nullptr;
};

class Player : public Screen {
  std::shared_ptr<PlayerInternal> internal;
  std::thread renderThread;
  PlayerConfiguration configuration;

public:
  /**
   * Creates a player with width, height and channels option.
   */
  Player(PlayerConfiguration configuration);

  /**
   * Draws the current frame to the screen.
   */
  void drawFrame();

  /**
   * Uploads new data to the GPU.
   **/
  void uploadFrame(char *scr = nullptr);

  /**
   * Returns wether the user has issue the command to close the player.
   **/
  bool shouldClose();

  /**
   * Launches an async thread to constantly update the contents of the texture.
   */
  void launch(char **scr);
};
