#include "util.hpp"

#include "yaml-cpp/yaml.h"

#include "scene.hpp"

Scene::Scene(std::unique_ptr<TextureAtlas> sceneAtlas) : tileSize(10.0f) {
    atlas = std::move(sceneAtlas);
}

void Scene::sortSprites() {
    std::sort(sprites.begin(), sprites.end(), [](const GameObject& a, const GameObject& b) {
        return a.zIndex < b.zIndex;
    });
}

void Scene::update(float dt) {

}