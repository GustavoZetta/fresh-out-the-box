#include "util.hpp"

#include "window.hpp"

const float Window::aspectRatioX = 16.0f;
const float Window::aspectRatioY = 9.0f;

Window::Window(const std::string &title, int w, int h) : m_title(title), width(w), height(h) {}

void Window::init() {
    Logger::info("Initializing window...");
    if (!glfwInit()) {
        Logger::error("GLFW could not initialize");
        return;
    }

    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    Logger::info("Creating GLFW Window...");
    m_winId = glfwCreateWindow(width, height, m_title.c_str(), NULL, NULL);
    if (m_winId == NULL) {
        Logger::error("Failed to create GLFW window");
        return;
    }
    // Making the OpenGL context as being this window
    glfwMakeContextCurrent(m_winId);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        Logger::error("Failed to initialize OpenGL context");
        return;
    }

    // This pointer is used to get information in callbacks.
    // It is set to this instance of window.
    glfwSetWindowUserPointer(m_winId, this);
    // Window resized callback
    glfwSetFramebufferSizeCallback(m_winId, s_framebufferSizeCallback);
    glfwSetCursorPosCallback(m_winId, s_cursorPosCallback);

    // Set correct OpenGL viewport in the first program size
    s_framebufferSizeCallback(m_winId, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    Logger::info("Window initialized!");
}

void Window::cleanup() {
    Logger::info("Destroying window...");
    glfwDestroyWindow(m_winId);
}

void Window::poll() {
    glfwSwapBuffers(m_winId);
    glfwPollEvents();
}

void Window::showWindow() {
    glfwShowWindow(m_winId);
    glfwMakeContextCurrent(m_winId);
}

bool Window::isKeyPressed(int key) {
    return glfwGetKey(m_winId, key);
}

bool Window::isMouseButtonPressed(int button) {
    bool current = glfwGetMouseButton(m_winId, button) == GLFW_PRESS;
    bool wasPressed = m_prevMouseButtons[button];
    m_prevMouseButtons[button] = current;
    return current && !wasPressed;
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(m_winId);
}

void Window::setTitle(const std::string &t) {
    glfwSetWindowTitle(m_winId, t.c_str());
    m_title = t;
}

std::string Window::getTitle() {
    return m_title;
}

void Window::s_framebufferSizeCallback(GLFWwindow *winId, int w, int h) {
    // Window User Pointer is a way in GLFW to store a pointer,
    // in this case, the Window class pointer.
    Window *winObj = (Window *)glfwGetWindowUserPointer(winId);

    winObj->height = h;
    winObj->width = w;

    // Math to setting up the glViewport to the center of the screen
    const float targetRatio = aspectRatioX / aspectRatioY;

    int viewW, viewH;

    viewW = w;
    viewH = (int)(w / targetRatio);

    if (viewH > h) {
        viewH = h;
        viewW = (int)(h * targetRatio);
    }

    int offsetX = (w - viewW) / 2;
    int offsetY = (h - viewH) / 2;

    glViewport(offsetX, offsetY, viewW, viewH);
}

void Window::s_cursorPosCallback(GLFWwindow *winId, double x, double y) {
    // Window User Pointer is a way in GLFW to store a pointer,
    // in this case, the Window class pointer.
    Window *winObj = (Window *)glfwGetWindowUserPointer(winId);

    winObj->mouseX = x;
    winObj->mouseY = y;
}

GLFWwindow *Window::getWindowID() {
    return m_winId;
}


glm::vec2 Window::getMouseGameCoords(float gameWidth, float gameHeight) {
    float normalX = (float)mouseX / width;
    float normalY = (float)mouseY / height;

    // converte para coordenadas do jogo
    return glm::vec2(normalX * gameWidth, normalY * gameHeight);
}