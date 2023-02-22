#pragma once
#include "input.hpp"
#include <imgui.h>

class Window : public InputManager {

  GLFWwindow *window;
  ImGuiContext *imgui;
  static GLFWwindow *createWindow(int width, int height, const char *name);
  static GLFWwindow *currentWindow;
  static ImGuiContext *currentImgui;

public:
  Window(int width, int height, const char *name);
  ~Window();

  void makeContextCurrent(bool force = false);

  void clearColor(float r, float g, float b, float a);
  void clear();

  void swapBuffers();
  bool shouldClose();
  bool shouldDraw();

  void beginImgui();
  void endImgui();

  GLFWwindow *getNativeWindow();
};
