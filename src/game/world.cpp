#include "../core/resourcemanager.hpp"
#include "../core/util.hpp"

#include "world.hpp"

World::World() : player(nullptr) {
    player = std::make_unique<Player>();
    bg = std::make_unique<GameObject>();
    bg->storeSprite(ResourceManager::loadSprite(Common::getContentPath() + "/assets/bg.png", true));
    bg->size = glm::vec2(800.0f, 600.0f);

    GameObject obj;
    obj.storeSprite(ResourceManager::loadSprite(Common::getContentPath() + "/assets/enemy.png", true));

    obj.size = glm::vec2(100.0f);
    obj.position = glm::vec2(400.0f, 300.0f);

    objects.emplace_back(std::move(obj));
}

void World::update(float deltaTime) {
    player->update(deltaTime);
    bg->update(deltaTime);
    for (GameObject &obj : objects) {
        obj.update(deltaTime);
    }
}
