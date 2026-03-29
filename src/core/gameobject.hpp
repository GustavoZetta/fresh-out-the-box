#pragma once

#include <glm/common.hpp>

#include <memory>

#include "core/renderer/sprite/animatedsprite.hpp"
#include "core/renderer/sprite/staticsprite.hpp"
#include "core/renderer/sprite/textureatlas.hpp"

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

    glm::vec4 color;

    glm::vec2 position;
    glm::vec2 size;

    float rotation;

    int zIndex;

  public:
    GameObject();

    // Sets the sprType variable to SpriteType::STATIC_SPRITE
    void setSprite(std::unique_ptr<StaticSprite> staticSprite);
    // Sets the sprType variable to SpriteType::ANIMATED_SPRITE
    void setSprite(std::unique_ptr<AnimatedSprite> animatedSprite);
    // Sets the sprType variable to SpriteType::TEXTURE_ATLAS
    void setAtlas(TextureAtlas *textureAtlas);

    unsigned int getSpriteID();

    // Should be called every frame
    virtual void update(float deltaTime);
};