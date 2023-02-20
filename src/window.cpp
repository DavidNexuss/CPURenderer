
#include <GL/glew.h>
// include
#include "GLFW/glfw3.h"
#include "window.hpp"
#include <GL/gl.h>

static void glfwFence() {
  static bool isInitialized = false;
  if (!isInitialized) {
    isInitialized = true;
    glfwInit();
  }
}

GLFWwindow *Window::createWindow(int width, int height, const char *name) {
  glfwFence();
  return glfwCreateWindow(width, height, name, nullptr, nullptr);
}
Window::Window(int width, int height, const char *name)
    : InputManager(window = createWindow(width, height, name)) {

  makeContextCurrent();
  glewInit();
}

void Window::swapBuffers() { glfwSwapBuffers(window); }
bool Window::shouldClose() { return glfwWindowShouldClose(window); }
bool Window::shouldDraw() { return true; }

void Window::makeContextCurrent(bool force) {
  static GLFWwindow *currentContext = nullptr;
  if (currentContext != window || force) {
    currentContext = window;
    glfwMakeContextCurrent(window);
  }
}
GLFWwindow *Window::getNativeWindow() { return window; }
