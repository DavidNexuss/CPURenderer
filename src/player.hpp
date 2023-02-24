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
};

class Player : public Window {
  GLuint vbo;
  GLuint program;
  std::vector<GLuint> playerTexture;

  void checkTextureSupport(int index);

protected:
  PlayerConfiguration configuration;

public:
  /**
   * Creates a player with width height and special configuration
   */
  Player(int width, int height,
         PlayerConfiguration configuration = PlayerConfiguration());

  /**
   * Draws texture with the given index
   * @brief drawTexture
   * @param text The texture index
   */
  void drawTexture(int text = 0);

  /**
   * Uploads texture with the given data and index
   * @brief uploadTexture
   * @param scr The texture data
   * @param text The texture index
   */
  void uploadTexture(TextureFormat scr, int text = 0);

  std::thread launch(TextureFormat scr);
};
