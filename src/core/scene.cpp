#include "scene.hpp"
#include "util.hpp"
#include "yaml-cpp/yaml.h"

Scene::Scene(std::unique_ptr<TextureAtlas> sceneAtlas) {
    atlas = std::move(sceneAtlas);
}

void Scene::update(float dt) {

}