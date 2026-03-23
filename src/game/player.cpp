#include "core/resourcemanager.hpp"
#include "core/util.hpp"

#include "core/game.hpp"
#include "game/player.hpp"

Player::Player() : GameObject(), playerSpeed(Game::objectSize * 2), walking(false) {
    std::unique_ptr<AnimatedSprite> animSpr = ResourceManager::loadAnimatedSprite(
        Common::getContentPath("/assets/walking.yml"),
        true);
    animSpr->setState("idle");
    setSprite(std::move(animSpr));

    size = glm::vec2(Game::objectSize);
}

void Player::setWalking(bool value) {
    if (value == walking) {
        return;
    }

    if (value) {
        walking = true;
        animatedSprite->setState("walking");
    } else {
        walking = false;
        animatedSprite->setState("idle");
    }
}