#pragma once
#include "gl.hpp"
#include "screen.hpp"
#include "window.hpp"
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

class Player : public Screen, public Window {
  std::thread renderThread;
  PlayerConfiguration configuration;

  GLuint vbo;
  GLuint program;
  GLuint playerTexture;
  GLuint texture;

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
   * Uploads new data to the GPU. If src is null, defaultFrameData will be used
   *instead
   */
  void uploadFrame(char *scr = nullptr);

  /**
   * Launches an async thread to constantly update the contents of the texture.
   */
  void launch(char **scr);
};
