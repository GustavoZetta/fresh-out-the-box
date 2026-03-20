#pragma once

#include <vector>

#include "core/gameobject.hpp"
#include "core/scene.hpp"

#include "game/player.hpp"

class World {
  public:
    std::unique_ptr<Player> player;
    std::unique_ptr<GameObject> bg;
    std::unique_ptr<Scene> scene;
    std::vector<GameObject> objects;

  public:
    World();

    void update(float deltaTime);
};