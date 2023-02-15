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

int main() {
  // Create window
  glfwInit();

  GLFWwindow *window = glfwCreateWindow(800, 800, "Player", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSetWindowSizeCallback(window, window_size_callback);
  glewInit();

  LOG("-> Window created\n");

  // Opengl setup
  GLuint program =
      compileProgram({compileShader("video.fs", GL_FRAGMENT_SHADER),
                      compileShader("video.vs", GL_VERTEX_SHADER)});

  GLuint vbo = uploadBuffer((void *)screenMesh, sizeof(screenMesh));

  // Opengl binding
  glUseProgram(program);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, (void *)0);

  // Read player configuration

  LOG("-> Awaiting input...\n");
  PlayerConfiguration configuration;
  read(0, &configuration, sizeof(configuration));

  vector<unsigned char> frame(configuration.textureWidth *
                              configuration.textureHeight *
                              configuration.textureChannels);
  GLuint playerTexture;
  bool reachEnd = false;
  while (!glfwWindowShouldClose(window)) {
    // Read next frame
    if (!reachEnd) {
      int status = readall(0, frame.data(), frame.size());
      if (status != frame.size() && status != 0) {
        LOG("-> Malformed frame! %d, expected %lu\n", status, frame.size());
        break;
      }
      if (status > 0) {
        playerTexture = uploadTexture(
            configuration.textureWidth, configuration.textureHeight,
            configuration.textureChannels, frame.data());
      } else {
        reachEnd = true;
        LOG("-> Reach EOF\n");
      }
    }
    // Draw frame
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
}
