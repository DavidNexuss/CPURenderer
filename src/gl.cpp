#include "debug.hpp"
#include <GL/glew.h>
#include <cstdio>
// include
#include "gl.hpp"

GLuint gl::compileShader(const char *content, GLenum type) {
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
GLuint gl::compileProgram(const std::vector<GLuint> &shaders) {
  GLuint program = glCreateProgram();
  for (auto sha : shaders) {

    if (sha == -1) {
      LOG("-> Shader crashed\n");
      return -1;
    }
    glAttachShader(program, sha);
  }
  glLinkProgram(program);
  LOG("-> Program compiled\n");
  return program;
}

// Uploads a buffer
GLuint gl::uploadBuffer(void *data, int count) {
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, count, data, GL_STATIC_DRAW);
  return buffer;
}

GLuint gl::uploadTexture(int width, int height, int channels, bool isHDR,
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
