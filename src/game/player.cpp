#include "../core/resourcemanager.hpp"
#include "../core/util.hpp"

#include "player.hpp"

Player::Player() : GameObject() {
    storeSprite(ResourceManager::loadSprite(Common::getContentPath() + "/assets/walking.png", Common::getContentPath() + "/assets/walking.yml", true));

    size = glm::vec2(100.0f);
}