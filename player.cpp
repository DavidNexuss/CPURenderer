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
const static float screenMesh[] = {
  -1, -1,
  -1, 1,
  1, 1,

  1, 1,
  -1, -1,
  1, -1
};
// clang-format on

string readFile(const char *path) {
  ifstream ifs(path);
  ifs.open(path);
  stringstream strStream;
  strStream << ifs.rdbuf();
  LOG("-> File read %s\n", path);
  return strStream.str();
}

// OpenGL boilerplate
GLuint compileShader(const char *path, GLenum type) {
  string data = readFile(path);
  GLuint shader = glCreateShader(type);
  const char *content = data.c_str();
  int len = data.size();
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

    LOG("-> Shader compiled %s\n", path);
  }
  return shader;
}

GLuint compileProgram(const std::vector<GLuint> &shaders) {
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

GLuint uploadBuffer(void *data, int count) {
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, count, data, GL_STATIC_DRAW);
  return buffer;
}
void window_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

GLuint uploadTexture(int width, int height, int channels, void *data) {
  static GLuint texture = -1;

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

struct PlayerInternal {

  GLFWwindow *window;
  GLuint vbo;
  GLuint program;
  GLuint playerTexture;

public:
  PlayerInternal() {
    // Create window
    glfwInit();

    window = glfwCreateWindow(640, 480, "Player", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glewInit();

    LOG("-> Window created\n");

    // Opengl setup
    program = compileProgram({compileShader("video.fs", GL_FRAGMENT_SHADER),
                              compileShader("video.vs", GL_VERTEX_SHADER)});

    vbo = uploadBuffer((void *)screenMesh, sizeof(screenMesh));

    // Opengl binding
    glUseProgram(program);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, (void *)0);
  }

  void drawFrame() {
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  void uploadFrame(char *data, int width, int height, int ch) {
    playerTexture = uploadTexture(width, height, ch, data);
  }

  bool shouldClose() { return glfwWindowShouldClose(window); }
};

bool Player::shouldClose() { return internal->shouldClose(); }
void Player::drawFrame() { internal->drawFrame(); }
void Player::uploadFrame(char *data, int width, int height, int ch) {
  internal->uploadFrame(data, width, height, ch);
}

Player::Player() : internal(std::make_unique<PlayerInternal>()) {}

#ifdef CLIENT
int main() {

  Player player;
  LOG("-> Awaiting input...\n");
  PlayerConfiguration configuration;
  read(0, &configuration, sizeof(configuration));

  vector<unsigned char> frame(configuration.textureWidth *
                              configuration.textureHeight *
                              configuration.textureChannels);

  bool reachEnd = false;
  while (!player.shouldClose()) {

    if (!reachEnd) {
      int status = readall(0, frame.data(), frame.size());
      if (status == 0) {
        LOG("EOF\n");
        reachEnd = true;
      } else if (status != frame.size()) {
        LOG("Bad frame\n");
        break;
      } else {
        player.uploadFrame((char *)frame.data(), configuration.textureWidth,
                           configuration.textureHeight,
                           configuration.textureChannels);
      }
    }
    player.drawFrame();
  }
}
#endif

#include <iostream>
#include <thread>
char *Player::launch(int width, int height, int ch) {
  static std::vector<char> data(width * height * ch);
  char *scr = data.data();

  glfwMakeContextCurrent(nullptr);
  renderThread = std::thread([=]() {
    LOG("-> Rendering thread launched\n");
    glfwMakeContextCurrent(this->internal->window);
    while (!this->internal->shouldClose()) {
      this->uploadFrame(scr, width, height, ch);
      this->drawFrame();
    }
    LOG("-> Window closed, rendering thread stopped\n");
  });

  return scr;
}
