#pragma once

#include "core/gameobject.hpp"

class Player : public GameObject {
  public:
    Player();

    float playerSpeed;
};