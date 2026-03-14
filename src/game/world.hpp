#pragma once

#include "../core/gameobject.hpp"
#include "player.hpp"
#include <vector>

class World {
  public:
    std::unique_ptr<Player> player;
    std::unique_ptr<GameObject> bg;
    std::vector<GameObject> objects;

  public:
    World();

    void update(float deltaTime);
};