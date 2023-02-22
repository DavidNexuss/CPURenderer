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

Player::Player(TextureFormat format, PlayerConfiguration configuration)
    : Window(format.width, format.height, "Player") {

  // Screen configuration
  this->configuration = configuration;
  this->format = format;

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
Player::Player(int width, int height) : Player(TextureFormat(width,height) ,PlayerConfiguration{}) { }

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
      if (playerTexture.size() > 1) {
        clear();
      }
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
  playerTexture[textureIndex] =
      gl::uploadTexture(format.width, format.height, format.channels,
                        format.channelsSize, data, playerTexture[textureIndex]);
}
void Player::uploadFrame(char *data) { uploadFrame(0, data); }
void Player::uploadFrame(int texture, TextureFormat scr) {
  makeContextCurrent();
  checkTextureSupport(texture);
  playerTexture[texture] = gl::uploadTexture(scr.width, scr.height, scr.channels, scr.channelsSize, (char*)scr.source, playerTexture[texture]);
}
void Player::uploadFrame(TextureFormat scr) {
  uploadFrame(0, scr);
}
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
    clearColor(0, 0, 0, 0);
  }
}
