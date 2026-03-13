
#include "gameobject.hpp"

GameObject::GameObject() : sprite(nullptr), color(glm::vec3(1.0f)), position(glm::vec2(0.0f)), size(glm::vec2(0.0f)), rotation(0.0f) {}

void GameObject::storeSprite(Sprite s) {
    sprite = std::make_unique<Sprite>(std::move(s));
}

void GameObject::moveTo(glm::vec2 pos) {
    position = pos;
}