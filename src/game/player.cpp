#include "core/resourcemanager.hpp"
#include "core/util.hpp"

#include "game/player.hpp"

Player::Player() : GameObject(), playerSpeed(100.0f) {
    std::unique_ptr<AnimatedSprite> animatedSpr = ResourceManager::loadSprite(
        Common::getContentPath() + "/assets/walking.png",
        Common::getContentPath() + "/assets/walking.yml",
        true
    );
    setSprite(std::move(animatedSpr));

    size = glm::vec2(100.0f);
}