#include "native/glglfw.h"

#include "core/util.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#include "core/resourcemanager.hpp"
#include "core/window.hpp"

#include "core/game.hpp"

const float Game::objectSize = 20.0f;

Game::Game()
    : window(nullptr),
      renderer(nullptr),
      world(nullptr),
      m_running(false) {}

// Game loop functions

void Game::processInput(float deltaTime) {
    if (window->isKeyPressed(GLFW_KEY_W)) {
        world->player->position += glm::vec2(0.0f, -1.0f) * world->player->playerSpeed * deltaTime;
    }
    if (window->isKeyPressed(GLFW_KEY_A)) {
        world->player->position += glm::vec2(-1.0f, 0.0f) * world->player->playerSpeed * deltaTime;
    }
    if (window->isKeyPressed(GLFW_KEY_S)) {
        world->player->position += glm::vec2(0.0f, 1.0f) * world->player->playerSpeed * deltaTime;
    }
    if (window->isKeyPressed(GLFW_KEY_D)) {
        world->player->position += glm::vec2(1.0f, 0.0f) * world->player->playerSpeed * deltaTime;
    }
    if (window->isKeyPressed(GLFW_KEY_F4)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (window->isKeyPressed(GLFW_KEY_F5)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Game::update(float deltaTime) {
    world->update(deltaTime);
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT); // Can be GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT

    renderer->drawObject(world->bg.get());
    for (GameObject &obj : world->objects) {
        renderer->drawObject(&obj);
    }
    renderer->drawObject(world->player.get());
}

// Game logic functions

void Game::init() {
    Logger::log("Initializing Game...");

    window = std::make_unique<Window>("Fresh out the box", 800, 450);
    renderer = std::make_unique<SpriteRenderer>();

    window->init();
    renderer->init(ResourceManager::loadShader(Common::getContentPath() + "/shaders/vertex.glsl", Common::getContentPath() + "/shaders/fragment.glsl"));

    world = std::make_unique<World>();

    Logger::log("Showing Window...");
    window->showWindow();

    Logger::log("Game Initialized!");
    Logger::log("--------------------------------");
}

void Game::run() {
    Logger::log("Starting Game Loop...");
    const std::string wt = window->getTitle();

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 450.0f, 0.0f, -1.0f, 1.0f); // needs to be in the same AspectRatio as the window
    renderer->shader->setMat4x4("projection", projection, true);

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
            float deltaTime = std::chrono::duration<float>(delta).count();

            const std::string title = wt + std::string(" - FPS: ") + std::to_string(fps);
            window->setTitle(title);

            processInput(deltaTime);

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