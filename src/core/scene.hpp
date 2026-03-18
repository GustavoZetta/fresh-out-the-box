#pragma once

#include "gameobject.hpp"
#include <vector>

class Scene {
  public:
    std::unique_ptr<TextureAtlas> atlas;
    std::vector<GameObject> objects;

  public:
    Scene(std::unique_ptr<TextureAtlas> atlas);

    void update(float deltaTime);
};