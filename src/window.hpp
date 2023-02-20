#pragma once
#include "input.hpp"

class Window : public InputManager {

  GLFWwindow *window;
  static GLFWwindow *createWindow(int width, int height, const char *name);

public:
  Window(int width, int height, const char *name);

  void makeContextCurrent(bool force = false);
  void swapBuffers();
  bool shouldClose();
  bool shouldDraw();

  GLFWwindow *getNativeWindow();
};
