#include "native/glglfw.h"

#include "core/util.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#include "core/resourcemanager.hpp"
#include "core/window.hpp"

#include "core/game.hpp"

const float Game::objectSize = 64.0f;
const float Game::gameWidth = 640.0f;
const float Game::gameHeight = 360.0f;

Game::Game()
    : window(nullptr),
      renderer(nullptr),
      world(nullptr),
      m_running(false),
      m_shouldStop(false),
      state(GameState::MAIN_MENU) {}

// Game loop functions

bool isPointInObject(const GameObject& obj, glm::vec2 point) {
    return point.x >= obj.position.x &&
           point.x <= obj.position.x + obj.size.x &&
           point.y >= obj.position.y &&
           point.y <= obj.position.y + obj.size.y;
}

void Game::processInput(float deltaTime) {

    glm::vec2 mousePos = glm::vec2(0.0f);

    switch (state) {
    case GameState::MAIN_MENU:
        mousePos = window->getMouseGameCoords(Game::gameWidth, Game::gameHeight);

        for (MenuButton &btn : mainMenu->buttons) {
            if (isPointInObject(btn, mousePos)) {
                if (window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
                    if (btn.action) {
                        btn.action();
                    }
                } else {
                    if (btn.enterMouse) {
                        btn.enterMouse();
                    }
                }
                
                break;
            } else {
                if (btn.exitMouse) {
                    btn.exitMouse();
                }
            }
        }
        break;
    case GameState::PLAYING:
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

        if (window->isKeyPressed(GLFW_KEY_W) || window->isKeyPressed(GLFW_KEY_A) || window->isKeyPressed(GLFW_KEY_S) || window->isKeyPressed(GLFW_KEY_D)) {
            world->player->setWalking(true);
        } else {
            world->player->setWalking(false);
        }

        if (window->isKeyPressed(GLFW_KEY_ESCAPE)) {
            state = GameState::MAIN_MENU;
        }
        break;
    case GameState::PAUSED:

        break;
    case GameState::GAME_OVER:

        break;
    }
}

void Game::update(float deltaTime) {
    world->update(deltaTime);
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT); // Can be GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT

    switch (state) {
    case GameState::MAIN_MENU:
        renderer->drawObject(world->bg.get());
        for (MenuButton &btn : mainMenu->buttons) {
            renderer->drawObject(&btn);
        }

        break;
    case GameState::PLAYING:
        renderer->drawObject(world->bg.get());
        for (GameObject &obj : world->scene->sprites) {
            renderer->drawObject(&obj);
        }
        for (GameObject &obj : world->objects) {
            renderer->drawObject(&obj);
        }
        renderer->drawObject(world->player.get());
        
        for (ParticleEmitter &emitter : world->emitters) {
            renderer->drawParticles(&emitter);
        }

        break;
    case GameState::PAUSED:

        break;
    case GameState::GAME_OVER:

        break;
    }
}

// Game logic functions

void Game::initMenu() {
    mainMenu = std::make_unique<Menu>(ResourceManager::loadTextureAtlas(Common::getContentPath("/assets/menus/buttons.yml"), true));

    glm::vec2 buttonSize = glm::vec2(100.0f, 60.0f);

    MenuButton button1;
    button1.setAtlas(mainMenu->menuAtlas.get());
    button1.atlasKey = "play";
    button1.size = buttonSize;
    button1.position = glm::vec2((Game::gameWidth - buttonSize.x) / 2, Game::gameHeight - (buttonSize.y * 2 + 30.0f));
    button1.enterMouse = [this]() {
        MenuButton* btn = &mainMenu->buttons.at(0);

        if (btn->atlasKey != "play_1") {
            btn->atlasKey = "play_1";
        }
    };
    button1.exitMouse = [this]() {
        MenuButton* btn = &mainMenu->buttons.at(0);

        if (btn->atlasKey != "play") {
            btn->atlasKey = "play";
        }
    };
    button1.action = [this]() {
        state = GameState::PLAYING;
    };

    MenuButton button2;
    button2.setAtlas(mainMenu->menuAtlas.get());
    button2.atlasKey = "quit";
    button2.size = buttonSize;
    button2.position = glm::vec2((Game::gameWidth - buttonSize.x) / 2, Game::gameHeight - (buttonSize.y + 20.0f));
    button2.enterMouse = [this]() {
        MenuButton* btn = &mainMenu->buttons.at(1);

        if (btn->atlasKey != "quit_1") {
            btn->atlasKey = "quit_1";
        }
    };
    button2.exitMouse = [this]() {
        MenuButton* btn = &mainMenu->buttons.at(1);

        if (btn->atlasKey != "quit") {
            btn->atlasKey = "quit";
        }
    };
    button2.action = [this]() {
        stop();
    };

    mainMenu->buttons.push_back(std::move(button1));
    mainMenu->buttons.push_back(std::move(button2));
}

void Game::init() {
    Logger::info("Initializing Game...");

    window = std::make_unique<Window>("Fresh out the box", 1280, 720);
    renderer = std::make_unique<SpriteRenderer>();

    window->init();
    renderer->init(ResourceManager::loadShader(Common::getContentPath() + "/shaders/vertex.glsl", Common::getContentPath() + "/shaders/fragment.glsl"));

    initMenu();
    world = std::make_unique<World>();

    Logger::info("Showing Window...");
    window->showWindow();

    setFramerate(0);

    Logger::info("Game Initialized!");
    Logger::log("--------------------------------");
}

void Game::run() {
    Logger::info("Starting Game Loop...");
    const std::string wt = window->getTitle();

    glm::mat4 projection = glm::ortho(0.0f, Game::gameWidth, Game::gameHeight, 0.0f, -1.0f, 1.0f); // needs to be in the same AspectRatio as the window
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
            delta = elapsedTime;
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

        if (m_shouldStop || window->shouldClose()) {
            cleanup();
        }
    }
    Logger::info("Game Loop finalized!");
}

void Game::stop() {
    if (m_shouldStop) {
        return;
    }
    m_shouldStop = true;
}

void Game::cleanup() {
    if (!m_running) {
        return;
    }
    m_running = false;
    Logger::info("Stopping Game!");
    window->cleanup();
    renderer->cleanup();
    Logger::info("Terminating GLFW...");
    glfwTerminate();
}

// "Utils" functions

bool Game::isRunning() {
    return m_running;
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