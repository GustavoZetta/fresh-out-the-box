#pragma once

#include <memory>
#include <vector>

#include "shader.hpp"
#include "sprite.hpp"
#include "texture.hpp"

class SpriteRenderer {
  public:
    SpriteRenderer();

    void createSprite();

    void init();
    void render();
    void cleanup();

  private:
    std::unique_ptr<Shader> shader;

    std::vector<Sprite> spriteList;
};