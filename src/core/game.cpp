#include "../native/glglfw.h"

#include <chrono>

#include "game.hpp"
#include "logger.hpp"
#include "window.hpp"

Game::Game(bool d)
    : window(nullptr),
      renderer(nullptr),
      debug(d),
      m_running(false) {}

// Game loop functions

void Game::processInput() {
    if (window->isKeyPressed(GLFW_KEY_W)) {
        value += 0.01f;
    }
}

void Game::update(float deltaTime) {
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT); // Can be GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
    renderer->render();
}

// Game logic functionsa

void Game::init() {
    Logger::log("Initializing Game...");

    window = std::make_unique<Window>("Bad Ideas Game", 400, 300);
    renderer = std::make_unique<SpriteRenderer>();

    window->init();
    renderer->init();

    Logger::log("Showing Window...");
    window->showWindow();

    Logger::log("Game Initialized!");
    Logger::log("--------------------------------");
}

void Game::run() {
    Logger::log("Starting Game Loop...");
    const std::string wt = window->getTitle();

    auto lastTime = std::chrono::steady_clock::now();

    auto accumulator = std::chrono::nanoseconds(0);
    auto frameAccumulator = std::chrono::nanoseconds(0);

    auto delta = std::chrono::nanoseconds(0);

    int frames = 0;
    bool canRender = false;

    m_running = true;

    while (isRunning()) {
        canRender = false;

        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime);
        lastTime = currentTime;

        frameAccumulator += elapsedTime;

        if (m_frametime > ONE_NANO) { // If frametime > 1 nano seconds, that means that the framerate isn't unlimited
            accumulator += elapsedTime;

            if (accumulator >= m_frametime) {
                delta = accumulator;

                long long remove = accumulator / m_frametime; // Gets the amount of frames that should've    passed
                accumulator -= m_frametime * remove;
                canRender = true;
            }
        } else {
            canRender = true;
        }

        if (frameAccumulator >= NANOSECOND) {
            fps = frames;
            frames = 0;
            frameAccumulator = std::chrono::nanoseconds(0);
        }

        if (canRender) {
            const std::string title = wt + std::string(" - FPS: ") + std::to_string(fps);
            window->setTitle(title);

            processInput();

            float deltaTime = std::chrono::duration<float>(delta).count();
            update(deltaTime);

            render();
            window->poll();
            frames++;
        }
    }
    Logger::log("Game Loop finalized!");
}

void Game::stop() {
    if (!m_running) {
        return;
    }
    m_running = false;
    Logger::log("Stopping Game!");
    window->cleanup();
    renderer->cleanup();
    Logger::log("Terminating GLFW...");
    glfwTerminate();
}

// "Utils" functions

bool Game::isRunning() {
    return m_running && !window->shouldClose();
}

void Game::setFramerate(int framerate) {
    if (framerate > 0) {
        m_framerate = framerate;
        m_frametime = NANOSECOND / framerate;
    } else {
        m_framerate = 0;
        m_frametime = ONE_NANO;
    }
}