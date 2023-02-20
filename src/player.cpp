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

Player::Player(PlayerConfiguration configuration) : Window(800, 600, "Player") {

  // Screen configuration
  this->configuration = configuration;
  Screen::width = configuration.width;
  Screen::height = configuration.height;
  Screen::scr = (color *)configuration.defaultFrameData;

  // Create window
  LOG("-> Window created\n");

  // Opengl setup
  program = gl::compileProgram({gl::compileShader(video_fs, GL_FRAGMENT_SHADER),
                                gl::compileShader(video_vs, GL_VERTEX_SHADER)});

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
      glViewport(0, 0, InputManager::getWidth(), InputManager::getHeight());
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

  playerTexture = gl::uploadTexture(configuration.width, configuration.height,
                                    configuration.channels, configuration.isHDR,
                                    data, playerTexture);
}

void Player::launch(char **scr) {
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
