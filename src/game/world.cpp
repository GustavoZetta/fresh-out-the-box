#include "core/resourcemanager.hpp"
#include "core/util.hpp"

#include "game/card/cardloading.hpp"

#include "game/world.hpp"

#include "core/game.hpp"

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

    cardAtlas = ResourceManager::loadTextureAtlas(Common::getContentPath("/cards/cardatlas.yml"), true);

    cards = std::move(ResourceManager::loadCards(Common::getContentPath("/cards/card.yml"), cardAtlas.get()));

    cardPacks = std::move(ResourceManager::loadCardPacks(Common::getContentPath("/cards/cardpack.yml")));

    particleAtlas = ResourceManager::loadTextureAtlas(Common::getContentPath("/assets/particleatlas.yml"), true);

    EmitterConfig config;

    config.spawnRange = glm::vec2(4.0f, 4.0f);
    config.acceleration = glm::vec2(0.0f, 0.0f);
    config.position = glm::vec2(400.0f, 300.0f);
    config.velocityMin = glm::vec2(-180.0f, -80.0f);
    config.velocityMax = glm::vec2(180.0f, -40.0f);
    config.colorStart = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
    config.colorEnd = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    config.lifetimeMin = 0.5f;
    config.lifetimeMax = 1.5f;
    config.sizeStart = 16.0f;
    config.sizeEnd = 4.0f;
    config.emitRate = 500;

    ParticleEmitter emitter(config, 1000);

    emitter.setAtlas(particleAtlas.get());
    emitter.atlasKey = "ball";

    emitters.push_back(std::move(emitter));
}

void World::update(float deltaTime) {
    player->update(deltaTime);
    bg->update(deltaTime);
    for (GameObject &obj : objects) {
        obj.update(deltaTime);
    }
    for (ParticleEmitter &emitter : emitters) {
        emitter.update(deltaTime);
    }
}

std::string World::rollCardPack(std::string cardPackId) {
    return std::string("test");
}
