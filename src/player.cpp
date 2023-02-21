#include <GL/glew.h>
// include
#include <GL/gl.h>

#include "debug.hpp"
#include "gl.hpp"
#include "player.hpp"
#include "static_data.hpp"
#include "util.hpp"
#include "window.hpp"

using namespace std;

Player::Player(PlayerConfiguration configuration)
    : Window(configuration.width, configuration.height, "Player") {

  // Screen configuration
  this->configuration = configuration;
  Screen::width = configuration.width;
  Screen::height = configuration.height;
  Screen::scr = (color *)configuration.defaultFrameData;

  // Create window
  LOG("-> Window created\n");

  if (configuration.fragmentShaderOverride == nullptr)
    configuration.fragmentShaderOverride = video_fs;
  if (configuration.vertexShaderOverride == nullptr)
    configuration.vertexShaderOverride = video_vs;

  // Opengl setup
  program = gl::compileProgram(
      {gl::compileShader(configuration.fragmentShaderOverride,
                         GL_FRAGMENT_SHADER),
       gl::compileShader(configuration.vertexShaderOverride,
                         GL_VERTEX_SHADER)});

  vbo = gl::uploadBuffer((void *)screenMesh, sizeof(screenMesh));

  // Opengl binding
  glUseProgram(program);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, (void *)0);

  // Textures setup
  playerTexture.push_back(-1);
}

void Player::drawFrame(int textureIndex, bool hold) {
  makeContextCurrent();
  if (shouldDraw()) {
    if (signalViewportUpdate()) {
      glViewport(0, 0, InputManager::getWindowWidth(),
                 InputManager::getWindowHeight());
    }
    checkTextureSupport(textureIndex);
    gl::bindTexture(playerTexture[textureIndex]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwPollEvents();
    if (!hold) {
      swapBuffers();
      if (playerTexture.size() > 1)
        glClear(GL_COLOR_BUFFER_BIT);
    }
  }
}

void Player::drawFrame(bool hold) { drawFrame(0, hold); }

void Player::uploadFrame(int textureIndex, char *data) {
  makeContextCurrent();
  if (data == nullptr) {
    data = configuration.defaultFrameData;
  }
  checkTextureSupport(textureIndex);
  playerTexture[textureIndex] = gl::uploadTexture(
      configuration.width, configuration.height, configuration.channels,
      configuration.isHDR, data, playerTexture[textureIndex]);
}
void Player::uploadFrame(char *data) { uploadFrame(0, data); }

void Player::launch(char **scr) {
  if (scr == nullptr) {
    scr = &configuration.defaultFrameData;
  }
  glfwMakeContextCurrent(nullptr);
  renderThread = std::thread([=]() {
    LOG("-> Rendering thread launched\n");
    this->makeContextCurrent(true);
    while (!this->shouldClose()) {
      this->uploadFrame(*scr);
      this->drawFrame();
    }
    LOG("-> Window closed, rendering thread stopped\n");
  });
}

void Player::checkTextureSupport(int index) {
  playerTexture.resize(std::max(index + 1, int(playerTexture.size())), -1);
  if (playerTexture.size() > 1) {
    // Enable texture blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_ALPHA, GL_ZERO);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glClearColor(0, 0, 0, 0);
  }
}

PlayerConfiguration RenderPlayer::configure(int width, int height) {
  PlayerConfiguration configuration;
  configuration.width = width;
  configuration.height = height;
  return configuration;
}

RenderPlayer::RenderPlayer(int width, int height)
    : Player(configure(width, height)), screenBuffer(width * height) {
  Screen::scr = screenBuffer.data();
  configuration.defaultFrameData = (char *)screenBuffer.data();
}
