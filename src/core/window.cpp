
#include "window.hpp"
#include "util.hpp"

Window::Window(const std::string &title, int w, int h) : m_title(title), width(w), height(h) {}

void Window::init() {
    Logger::log("Initializing window...");
    if (!glfwInit()) {
        Logger::log("GLFW could not initialize");
        return;
    }

    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    Logger::log("Creating GLFW Window...");
    m_winId = glfwCreateWindow(width, height, m_title.c_str(), NULL, NULL);
    if (m_winId == NULL) {
        Logger::log("Failed to create GLFW window");
        return;
    }
    // Making the OpenGL context as being this window
    glfwMakeContextCurrent(m_winId);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        Logger::log("Failed to initialize OpenGL context");
    }

    // This pointer is used to get information in callbacks.
    // It is set to this instance of window.
    glfwSetWindowUserPointer(m_winId, this);
    // Window resized callback
    glfwSetFramebufferSizeCallback(m_winId, s_framebufferSizeCallback);
    glfwSetCursorPosCallback(m_winId, s_cursorPosCallback);

    glClearColor(0.55f, 0.05f, 0.6f, 1.0f);
    Logger::log("Window initialized!");
}

void Window::cleanup() {
    Logger::log("Destroying window...");
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

    // glViewport sets the size of the render so OpenGL can operate just fine.
    glViewport(0, 0, w, h);
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
