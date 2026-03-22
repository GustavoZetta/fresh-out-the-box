#include "core/gameobject.hpp"

GameObject::GameObject() : staticSprite(nullptr), animatedSprite(nullptr), textureAtlas(nullptr), color(glm::vec3(1.0f)), position(glm::vec2(0.0f)), size(glm::vec2(0.0f)), rotation(0.0f), zIndex(0) {}

unsigned int GameObject::getSpriteID() {
    switch (sprType) {
    case SpriteType::STATIC_SPRITE:
        return staticSprite->sprite->ID();
    case SpriteType::ANIMATED_SPRITE:
        return animatedSprite->sprite->ID();
    case SpriteType::TEXTURE_ATLAS:
        return textureAtlas->sprite->ID();
    
    default:
        return 0;
    }
}

void GameObject::update(float deltaTime) {
    if (sprType == SpriteType::ANIMATED_SPRITE) {
        animatedSprite->update(deltaTime);
    }
}

// Sprites setters

void GameObject::setSprite(std::unique_ptr<StaticSprite> staticSpr) {
    staticSprite = std::move(staticSpr);
    sprType = SpriteType::STATIC_SPRITE;

    animatedSprite = nullptr;
    textureAtlas = nullptr;
}

void GameObject::setSprite(std::unique_ptr<AnimatedSprite> animatedSpr) {
    animatedSprite = std::move(animatedSpr);
    sprType = SpriteType::ANIMATED_SPRITE;

    staticSprite = nullptr;
    textureAtlas = nullptr;
}

void GameObject::setAtlas(TextureAtlas *texAtlas) {
    textureAtlas = texAtlas;
    sprType = SpriteType::TEXTURE_ATLAS;

    staticSprite = nullptr;
    animatedSprite = nullptr;
}
