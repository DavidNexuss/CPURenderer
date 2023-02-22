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
   * Creates a player with width, height and channels option.
   */
  Player(int width, int height, PlayerConfiguration configuration = PlayerConfiguration());

  /**
   * Draws texture
   * @brief drawTexture
   * @param text
   */
  void drawTexture(int text = 0);

  /**
   * @brief uploadTexture
   * @param scr
   * @param text
   */
  void uploadTexture(TextureFormat scr,int text = 0);

  std::thread launch(TextureFormat scr);
};
