#include "input.hpp"
#include "GLFW/glfw3.h"

InputManager::InputManager(GLFWwindow *window) {
  glfwSetKeyCallback(window, InputManager::key_callback);
  glfwSetMouseButtonCallback(window, InputManager::mouse_button_callback);
  glfwSetCursorPosCallback(window, InputManager::cursor_position_callback);
  glfwSetWindowSizeCallback(window, InputManager::window_size_callback);
  glfwSetWindowUserPointer(window, this);
  this->m_needsViewportUpdate = true;
}

#define INP(window) (InputManager *)glfwGetWindowUserPointer(window);

void InputManager::key_callback(GLFWwindow *window, int key, int scancode,
                                int action, int mods) {
  auto input = INP(window);

  if (action == GLFW_PRESS || action == GLFW_REPEAT)
    input->m_keyPresses[key]++;
  if (action == GLFW_RELEASE)
    input->m_keyPresses[key] = 0;
}

void InputManager::cursor_position_callback(GLFWwindow *window, double xpos,
                                            double ypos) {
  auto input = INP(window);
  input->m_x = xpos;
  input->m_y = ypos;
}
void InputManager::mouse_button_callback(GLFWwindow *window, int button,
                                         int action, int mods) {
  return InputManager::key_callback(window, button, 0, action, mods);
}

void InputManager::window_size_callback(GLFWwindow *window, int width,
                                        int height) {
  auto input = INP(window);

  input->m_width = width;
  input->m_height = height;
  input->m_needsViewportUpdate = true;
}
bool InputManager::needsViewportUpdate() { return m_needsViewportUpdate; }
bool InputManager::signalViewportUpdate() {
  bool current = m_needsViewportUpdate;
  m_needsViewportUpdate = false;
  return current;
}

int InputManager::getWindowWidth() { return m_width; }
int InputManager::getWindowHeight() { return m_height; }

bool InputManager::isKeyJustPressed(int key) {
  bool result = m_keyPresses[key] == 1;
  if (result)
    m_keyPresses[key] = 2;
  return result;
}
bool InputManager::isKeyPressed(int key) { return m_keyPresses[key] > 0; }

double InputManager::getX() { return m_x; }
double InputManager::getY() { return m_y; }
