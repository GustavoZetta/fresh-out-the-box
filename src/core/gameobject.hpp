#pragma once

#include "sprite.hpp"
#include <memory>
#include <glm/common.hpp>

class GameObject {
  public:
    std::unique_ptr<Sprite> sprite;
    glm::vec3 color;

    glm::vec2 position;
    glm::vec2 size;

    float rotation;

  public:
    GameObject();

    GameObject(GameObject &&) = default;
    GameObject &operator=(GameObject &&) = default;

    // Picks up the ownership of the texture
    void storeSprite(Sprite sprite);
    void moveTo(glm::vec2 position);
};