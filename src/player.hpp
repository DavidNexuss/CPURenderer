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
  std::vector<GLuint> playerTexture;

  void checkTextureSupport(int index);

protected:
  PlayerConfiguration configuration;
  TextureFormat format;

public:
  /**
   * Creates a player with width, height and channels option.
   */
  Player(TextureFormat format, PlayerConfiguration configuration = PlayerConfiguration());
  Player(int width, int height);

  /**
   * Draws the current frame to the screen.
   */
  void drawFrame(bool hold = false);
  void drawFrame(int text, bool hold = false);

  /**
   * Uploads new data to the GPU. If src is null, defaultFrameData will be used
   *instead
   */
  void uploadFrame(char *scr = nullptr);
  void uploadFrame(int text, char *scr = nullptr);
  void uploadFrame(int text, TextureFormat scr);
  void uploadFrame(TextureFormat scr);

  /**
   * Launches an async thread to constantly update the contents of the texture.
   */
  void launch(char **scr = nullptr);
  void launch(int text, char **scr = nullptr);
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
    this->channelsSize = sizeof(T);
    this->format = *this;
    this->configuration.defaultFrameData = (char *)this->source;
  }
};

using RGBRenderPlayer = RenderPlayer<unsigned char, 3>;
