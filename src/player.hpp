#pragma once
#include "gl.hpp"
#include "screen.hpp"
#include "texture.hpp"
#include "window.hpp"
#include <thread>
#include <vector>

struct PlayerConfiguration {
  const char *fragmentShaderOverride = nullptr;
  const char *vertexShaderOverride = nullptr;
  const char *titleOverride = nullptr;
  char *defaultFrameData = nullptr;
};

class Player : public Window {
  std::thread renderThread;

  GLuint vbo;
  GLuint program;
  GLuint playerTexture;
  GLuint texture;

protected:
  PlayerConfiguration configuration;
  TextureFormat format;

public:
  /**
   * Creates a player with width, height and channels option.
   */
  Player(TextureFormat format,
         PlayerConfiguration configuration = PlayerConfiguration());

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
  void launch(char **scr = nullptr);

  inline int getScreenWidth() { return format.width; }
  inline int getScreenHeight() { return format.height; }
};

template <typename T, int channels>
class RenderPlayer : public Screen<T, channels>, public Player {
  std::vector<vec<T, channels>> texture;

public:
  RenderPlayer(int width, int height)
      : Screen<T, channels>(), Player(TextureFormat(width, height)),
        texture(width * height) {

    this->width = width;
    this->height = height;
    this->source = texture.data();
    this->channels = channels;
    this->channelsSize = sizeof(channels);
    this->format = *this;
    this->configuration.defaultFrameData = (char *)this->source;
  }
};

using RGBRenderPlayer = RenderPlayer<unsigned char, 3>;
