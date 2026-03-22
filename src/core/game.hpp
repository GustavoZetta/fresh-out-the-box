#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "core/renderer/spriterenderer.hpp"
#include "core/window.hpp"
#include "core/menu.hpp"

#include "game/world.hpp"

enum class GameState {
  MAIN_MENU,
  PLAYING,
  PAUSED,
  GAME_OVER
};

class Game {
  public:
    std::unique_ptr<Menu> mainMenu;

    std::unique_ptr<World> world;

    std::unique_ptr<Window> window;
    std::unique_ptr<SpriteRenderer> renderer;
    
    GameState state;

    const std::chrono::nanoseconds NANOSECOND = std::chrono::nanoseconds(1000000000i64);
    const std::chrono::nanoseconds ONE_NANO = std::chrono::nanoseconds(1);

    int fps = 0;

    static const float objectSize;

  public:
    // Starts the default variables and sets the
    // window title and the debug variable in the constructor.
    Game();

    // Initializes the menu and puts every button in it
    void initMenu();

    // Initializes window and renderer.
    void init();
    // Starts the game loop.
    // When game loop ends, it does not necessarily
    // sets m_running to false.
    void run();
    // Stops the engine
    // Sets m_running to false
    void stop();

    bool isRunning();

    // Setting anything below 1 will set the framerate to unlimited
    void setFramerate(int framerate);

  private:
    int m_framerate = 60;
    std::chrono::nanoseconds m_frametime = NANOSECOND / m_framerate;

    bool m_running;
    bool m_wireframe;

  private:
    void processInput(float deltaTime);
    void update(float deltaTime);
    void render();
};