#pragma once
#include <GL/gl.h>
#include <vector>
namespace gl {
GLuint compileShader(const char *content, GLenum type);
GLuint compileProgram(const std::vector<GLuint> &shaders);
GLuint uploadBuffer(void *data, int count);
GLuint uploadTexture(int width, int height, int channels, bool isHDR,
                     void *data, GLuint texture);

void bindTexture(GLuint texture, bool force = false);
} // namespace gl
