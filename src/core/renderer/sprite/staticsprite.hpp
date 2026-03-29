#pragma once

#include <glm/common.hpp>

#include <memory>

#include "core/renderer/sprite/sprite.hpp"

// Expects you to initialize the sprite
class StaticSprite {
  public:
    std::unique_ptr<Sprite> sprite;

    glm::vec2 uvSize;
    glm::vec2 uvOffset;
  public:
    StaticSprite(std::unique_ptr<Sprite> spr);
};