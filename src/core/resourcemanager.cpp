#include "native/glglfw.h"
#include "native/stb_image.h"

#include "core/util.hpp"

#include "yaml-cpp/yaml.h"

#include <fstream>
#include <memory>
#include <sstream>

#include "core/game.hpp"

#include "core/resourcemanager.hpp"

// Static Sprite

std::unique_ptr<StaticSprite> ResourceManager::loadStaticSprite(const std::string &imagePath, bool alpha) {
    std::unique_ptr<StaticSprite> staticSpr = std::make_unique<StaticSprite>(std::make_unique<Sprite>());

    if (alpha) {
        staticSpr->sprite->internal_format = GL_RGBA;
        staticSpr->sprite->img_format = GL_RGBA;
    }

    int width;
    int height;
    int nrChannels;

    unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, alpha ? 4 : 3);
    if (!data) {
        Logger::error("Texture loading error: " + imagePath);
        Logger::error("Reason: " + std::string(stbi_failure_reason()));
    }

    staticSpr->sprite->createSprite(width, height, data);
    stbi_image_free(data);

    return std::move(staticSpr);
}

// Animated Sprite

std::unique_ptr<AnimatedSprite> ResourceManager::loadAnimatedSprite(const std::string &configPath, bool alpha) {
    YAML::Node config = YAML::LoadFile(configPath);

    YAML::Node animations = config["animations"];

    if (!animations.IsMap()) {
        Logger::warn("Unable to load animated sprite config: " + configPath);
    }

    std::unique_ptr<AnimatedSprite> animSpr = std::make_unique<AnimatedSprite>(std::make_unique<Sprite>());

    std::string imagePath = Common::getContentPath(config["image_path"].as<std::string>("/assets/unknown.png"));

    if (alpha) {
        animSpr->sprite->internal_format = GL_RGBA;
        animSpr->sprite->img_format = GL_RGBA;
    }

    int width;
    int height;
    int nrChannels;

    unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, alpha ? 4 : 3);
    if (!data) {
        Logger::error("Texture loading error: " + imagePath);
        Logger::error("Reason: " + std::string(stbi_failure_reason()));
    }

    animSpr->sprite->createSprite(width, height, data);
    stbi_image_free(data);

    int gridsizex = config["gridsize_x"].as<int>(32);
    int gridsizey = config["gridsize_y"].as<int>(32);

    if (config["gridsize"].IsDefined()) {
        gridsizex = config["gridsize"].as<int>(32);
        gridsizey = gridsizex;
    }

    for (auto state : animations) {
        YAML::Node frames = state.second["frames"];

        if (!frames.IsSequence()) {
            Logger::warn("Unable to load animated sprite info from config: " + configPath);
        }

        AnimatedInfoList list;
        list.fps = state.second["fps"].as<int>(15);

        for (auto frame : frames) {
            int x = frame["x"].as<int>(0) * gridsizex;
            int y = frame["y"].as<int>(0) * gridsizey;
            int width = frame["width"].as<int>(1) * gridsizex;
            int height = frame["height"].as<int>(1) * gridsizey;

            list.animationFrameInfo.push_back(animSpr->calcUvInfo(x, y, width, height));
        }

        animSpr->addAnimationInfoList(state.first.as<std::string>(""), list);
    }

    return std::move(animSpr);
}

// Texture Atlas

std::unique_ptr<TextureAtlas> ResourceManager::loadTextureAtlas(const std::string &configPath, bool alpha) {
    YAML::Node config = YAML::LoadFile(configPath);

    YAML::Node tilesetInfo = config["tileset"];
    if (!tilesetInfo.IsMap()) {
        Logger::warn("Unable to load tileset info: " + configPath);
    }

    std::unique_ptr<TextureAtlas> atlas = std::make_unique<TextureAtlas>(std::make_unique<Sprite>(), static_cast<int>(tilesetInfo.size()));

    if (alpha) {
        atlas->sprite->internal_format = GL_RGBA;
        atlas->sprite->img_format = GL_RGBA;
    }

    std::string imagePath = Common::getContentPath(config["image_path"].as<std::string>("/assets/unknown.png"));

    int width;
    int height;
    int nrChannels;

    unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, alpha ? 4 : 3);
    if (!data) {
        Logger::error("Texture loading error: " + imagePath);
        Logger::error("Reason: " + std::string(stbi_failure_reason()));
    }

    atlas->sprite->createSprite(width, height, data);
    stbi_image_free(data);

    int gridsizex = config["gridsize_x"].as<int>(32);
    int gridsizey = config["gridsize_y"].as<int>(32);

    if (config["gridsize"].IsDefined()) {
        gridsizex = config["gridsize"].as<int>(32);
        gridsizey = gridsizex;
    }

    if (width % gridsizex != 0 || height % gridsizey != 0) {
        Logger::warn("Texture atlas does not have simetric grid, textures may be clipped");
        Logger::warn("Config path: " + configPath);
        Logger::warn("Image path: " + imagePath);
    }

    for (auto it : tilesetInfo) {
        std::string name = it.first.as<std::string>("");
        int x = it.second["x"].as<int>(0) * gridsizex;
        int y = it.second["y"].as<int>(0) * gridsizey;
        int width = it.second["width"].as<int>(1) * gridsizex;
        int height = it.second["height"].as<int>(1) * gridsizey;

        atlas->addUvInfo(name, x, y, width, height);
    }

    return std::move(atlas);
}

// Scene

std::unique_ptr<Scene> ResourceManager::loadScene(const std::string &scenePath, std::unique_ptr<TextureAtlas> sceneAtlas) {
    YAML::Node config = YAML::LoadFile(scenePath);

    std::string nullchar = config["nullchar"].as<std::string>("~");
    YAML::Node sceneInfo = config["sprites"];
    if (!sceneInfo.IsMap()) {
        Logger::warn("Unable to load scene info: " + scenePath);
    }

    std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::move(sceneAtlas));

    scene->sprites.reserve(32);

    for (auto it : sceneInfo) {
        int zIndex = it.second["z-index"].as<int>(0);
        std::string path = it.second["path"].as<std::string>("scene.scene");

        std::string line;
        std::ifstream fstream(Common::getContentPath() + path);
        if (fstream) {
            int y = 0;
            int x = 0;

            while (std::getline(fstream, line)) { // reads entire line
                std::string token;

                std::istringstream sstream(line);

                while (sstream >> token) { // reads each word separated by space
                    if (token != nullchar) {

                        GameObject object;

                        object.setAtlas(scene->atlas.get());
                        object.atlasKey = token;
                        object.zIndex = zIndex;

                        object.size = glm::vec2(Game::objectSize);
                        object.position = glm::vec2(
                            Game::objectSize * x,
                            Game::objectSize * y);

                        scene->sprites.push_back(std::move(object));
                    }
                    x++;
                }
                x = 0;
                y++;
            }
        }
    }

    scene->sortSprites();

    return std::move(scene);
}

// Shader

Shader ResourceManager::loadShader(const std::string &vertexPath, const std::string &fragmentPath) {
    std::string vertexSrc = getTextFromFile(vertexPath);
    std::string fragmentSrc = getTextFromFile(fragmentPath);

    Shader shader;
    shader.createShader(vertexSrc, fragmentSrc);

    return shader;
}

std::string ResourceManager::getTextFromFile(const std::string &vertexPath) {
    std::ifstream file(vertexPath);
    std::stringstream buffer;

    buffer << file.rdbuf();
    return buffer.str();
}