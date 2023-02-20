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
  playerTexture = -1;
}

void Player::drawFrame() {
  makeContextCurrent();
  if (shouldDraw()) {
    if (signalViewportUpdate()) {
      glViewport(0, 0, InputManager::getWindowWidth(),
                 InputManager::getWindowHeight());
    }
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwPollEvents();
    swapBuffers();
  }
}

void Player::uploadFrame(char *data) {
  makeContextCurrent();
  if (data == nullptr) {
    data = configuration.defaultFrameData;
  }

  playerTexture =
      gl::uploadTexture(format.width, format.height, format.channels,
                        format.channelsSize, data, playerTexture);
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
