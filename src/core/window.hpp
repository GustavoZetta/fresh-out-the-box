#pragma once

#include "native/glglfw.h"

#include <glm/common.hpp>

#include <string>
#include <unordered_map>

class Window {
  public:
    int height;
    int width;

    // Defines the GL Viewport aspect ratio
    const static float aspectRatioX;
    const static float aspectRatioY;

    double mouseX;
    double mouseY;

  public:
    // Window constructor (just initialize variables).
    Window(const std::string &title, int width, int height);

    // Initializes GLFW and GLAD and creates the window
    // with all the needed configurations.
    void init();
    // Destroys the GLFW window.
    void cleanup();
    // Poll GLFW events.
    // Should be called every frame.
    void poll();

    bool shouldClose();
    bool isKeyPressed(int key);
    bool isMouseButtonPressed(int button);

    void showWindow();
    void setTitle(const std::string &title);

    std::string getTitle();

    GLFWwindow *getWindowID();

    // An callback that sets the width and height of the window
    static void s_framebufferSizeCallback(GLFWwindow *win, int width, int height);
    // An callback that sets the mouseX and mouseY when its in the window
    static void s_cursorPosCallback(GLFWwindow *win, double mouseX, double mouseY);

    // Returns the mouse coords in game coords so its easier to use
    glm::vec2 getMouseGameCoords(float gameWidth, float gameHeight); 

  private:
    std::unordered_map<int, bool> m_prevMouseButtons;
    GLFWwindow *m_winId;

    std::string m_title;
};