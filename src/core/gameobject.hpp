#pragma once

#include <glm/common.hpp>

#include <memory>

#include "core/renderer/animatedsprite.hpp"
#include "core/renderer/staticsprite.hpp"
#include "core/renderer/textureatlas.hpp"

enum class SpriteType {
  STATIC_SPRITE,
  ANIMATED_SPRITE,
  TEXTURE_ATLAS
};

class GameObject {
  public:
    SpriteType sprType;

    std::unique_ptr<StaticSprite> staticSprite;
    std::unique_ptr<AnimatedSprite> animatedSprite;

    TextureAtlas* textureAtlas;
    std::string atlasKey;

    glm::vec3 color;

    glm::vec2 position;
    glm::vec2 size;

    float rotation;

    int zIndex;

  public:
    GameObject();

    // These also set the sprType variable
    void setSprite(std::unique_ptr<StaticSprite> staticSprite);
    void setSprite(std::unique_ptr<AnimatedSprite> animatedSprite);

    void setAtlas(TextureAtlas *textureAtlas);

    unsigned int getSpriteID();

    // Should be called every frame
    virtual void update(float deltaTime);
};