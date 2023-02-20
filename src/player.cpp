#include <GL/glew.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
// include
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "debug.hpp"
#include "player.hpp"
#include "util.hpp"

using namespace std;

// Dummy utility functions

// clang-format off

static const char* video_vs = "\
#version 330 core\n\
in vec2 pos;\n\
out vec2 fs_pos;\n\
void main() {\n\
  fs_pos = pos;\n\
  gl_Position = vec4(pos.x, pos.y, 0, 1);\n\
}\n\
" ;
static const char* video_fs = "\
#version 330 core\n\
out vec3 color;\n\
uniform sampler2D tex;\n\
in vec2 fs_pos;\n\
void main() {\n\
  color = texture(tex, fs_pos * 0.5 + 0.5).xyz; \n\
}\
";

const static float screenMesh[] = {
  -1, -1,
  -1, 1,
  1, 1,

  1, 1,
  -1, -1,
  1, -1
};
// clang-format on

// OpenGL boilerplate

// Compiles a shader
static GLuint compileShader(const char *content, GLenum type) {
  GLuint shader = glCreateShader(type);
  int len = strlen(content);
  glShaderSource(shader, 1, &content, &len);
  glCompileShader(shader);
  int InfoLogLength;
  int Result;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(shader, InfoLogLength, NULL,
                       &VertexShaderErrorMessage[0]);
    printf("%s\n", &VertexShaderErrorMessage[0]);
    LOG("-> Error compiling shader:\n%s\nERROR:\n%s\n", content,
        VertexShaderErrorMessage.data());
    return -1;
  } else {

    // LOG("-> Shader compiled %s\n", path);
  }
  return shader;
}

// Compiles a program
static GLuint compileProgram(const std::vector<GLuint> &shaders) {
  GLuint program = glCreateProgram();
  for (auto sha : shaders) {

    if (sha == -1) {
      LOG("-> Shader crashed\n");
      exit(1);
    }
    glAttachShader(program, sha);
  }
  glLinkProgram(program);
  LOG("-> Program compiled\n");
  return program;
}

// Uploads a buffer
static GLuint uploadBuffer(void *data, int count) {
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, count, data, GL_STATIC_DRAW);
  return buffer;
}

static GLuint uploadTexture(int width, int height, int channels, bool isHDR,
                            void *data, GLuint texture) {
  if (texture == -1) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    LOG("-> Texture created width: %d height: %d channels: %d\n", width, height,
        channels);
  } else {
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB,
                    GL_UNSIGNED_BYTE, data);
  }
  return texture;
}

/**
 * GLFW window callbacks
 */
static void window_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

struct PlayerInternal {

  GLFWwindow *window;
  GLuint vbo;
  GLuint program;
  GLuint playerTexture;
  GLuint texture;

public:
  PlayerInternal(const char *title) {
    // Create window
    glfwInit();

    window = glfwCreateWindow(960, 540, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glewInit();

    LOG("-> Window created\n");

    // Opengl setup
    program = compileProgram({compileShader(video_fs, GL_FRAGMENT_SHADER),
                              compileShader(video_vs, GL_VERTEX_SHADER)});

    vbo = uploadBuffer((void *)screenMesh, sizeof(screenMesh));

    // Opengl binding
    glUseProgram(program);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, (void *)0);

    // Textures setup
    playerTexture = -1;
  }

  void drawFrame() {
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  void uploadFrame(char *data, int width, int height, int ch, bool HDR) {
    playerTexture = uploadTexture(width, height, ch, HDR, data, playerTexture);
  }

  bool shouldClose() { return glfwWindowShouldClose(window); }
};

bool Player::shouldClose() { return internal->shouldClose(); }
void Player::drawFrame() { internal->drawFrame(); }
void Player::uploadFrame(char *data) {
  if (data == nullptr)
    data = configuration.defaultFrameData;

  internal->uploadFrame(data, configuration.width, configuration.height,
                        configuration.channels, configuration.isHDR);
}

Player::Player(PlayerConfiguration configuration)
    : internal(std::make_unique<PlayerInternal>("Player")) {
  this->configuration = configuration;
  Screen::width = configuration.width;
  Screen::height = configuration.height;
  Screen::scr = (color *)configuration.defaultFrameData;
}

void Player::launch(char **scr) {
  glfwMakeContextCurrent(nullptr);
  renderThread = std::thread([=]() {
    LOG("-> Rendering thread launched\n");
    glfwMakeContextCurrent(this->internal->window);
    while (!this->internal->shouldClose()) {
      this->uploadFrame(*scr);
      this->drawFrame();
    }
    LOG("-> Window closed, rendering thread stopped\n");
  });
}
