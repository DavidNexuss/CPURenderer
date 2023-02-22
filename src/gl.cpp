#include "debug.hpp"
#include <GL/glew.h>
#include <cstdio>
// include
#include "gl.hpp"
#include <GL/gl.h>

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

void gl::bindTexture(GLuint texture, bool force) {
  static GLuint currentTexture = -1;
  if (texture != currentTexture || force) {
    glBindTexture(GL_TEXTURE_2D, texture);
  }
  currentTexture = texture;
}

GLuint gl::uploadTexture(int width, int height, int channels, int channelSize, void *data, GLuint texture) {

  static GLuint formats[] = {
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA
  };

  static GLuint channelType[] = {
    GL_UNSIGNED_BYTE,
    GL_FLOAT
  };


  if (texture == -1) {
    int ch = channelSize > 1 ? 1 : 0;

    glGenTextures(1, &texture);
    gl::bindTexture(texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, formats[channels - 1], channelType[ch], data);
    LOG("-> Texture created width: %d height: %d channels: %d and HDR: %b\n", width, height, channels, ch);
  } else {
    gl::bindTexture(texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB,
                    GL_UNSIGNED_BYTE, data);
  }
  return texture;
}
