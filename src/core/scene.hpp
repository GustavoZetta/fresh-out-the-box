#pragma once

#include "gameobject.hpp"
#include <vector>

class Scene {
  public:
    std::unique_ptr<TextureAtlas> atlas;
    std::vector<GameObject> sprites;

    glm::vec2 tileSize;

  public:
    Scene(std::unique_ptr<TextureAtlas> atlas);

    void sortSprites();

    void update(float deltaTime);
};