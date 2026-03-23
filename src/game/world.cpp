#include "core/resourcemanager.hpp"
#include "core/util.hpp"

#include "world.hpp"

World::World() {
    player = std::make_unique<Player>();

    bg = std::make_unique<GameObject>();
    std::unique_ptr<StaticSprite> staticSpr = ResourceManager::loadStaticSprite(
        Common::getContentPath("/assets/bg.png"),
        false);
    bg->setSprite(std::move(staticSpr));
    bg->size = glm::vec2(640.0f, 360.0f);

    GameObject obj;
    staticSpr = ResourceManager::loadStaticSprite(
        Common::getContentPath("/assets/enemy.png"),
        true);
    obj.setSprite(std::move(staticSpr));

    obj.size = glm::vec2(100.0f);
    obj.position = glm::vec2(300.0f, 200.0f);

    objects.emplace_back(std::move(obj));

    scene = ResourceManager::loadScene(
        Common::getContentPath("/scenes/scene1.yml"),
        ResourceManager::loadTextureAtlas(
            Common::getContentPath("/assets/tilesets/test.yml"),
            true));
}

void World::update(float deltaTime) {
    player->update(deltaTime);
    bg->update(deltaTime);
    for (GameObject &obj : objects) {
        obj.update(deltaTime);
    }
}
