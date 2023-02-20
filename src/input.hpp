#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>

class InputManager {
  std::unordered_map<char, int> m_keyPresses;
  double m_x;
  double m_y;
  int m_width;
  int m_height;
  bool m_needsViewportUpdate;

  static void key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);

  static void cursor_position_callback(GLFWwindow *window, double xpos,
                                       double ypos);
  static void mouse_button_callback(GLFWwindow *window, int button, int action,
                                    int mods);

  static void window_size_callback(GLFWwindow *window, int width, int height);

public:
  InputManager(GLFWwindow *window);

  bool isKeyPressed(char key);
  bool isKeyJustPressed(char key);

  double getX();
  double getY();

  int getWidth();
  int getHeight();

  bool isMouseClicked();

  bool needsViewportUpdate();
  bool signalViewportUpdate();
};
