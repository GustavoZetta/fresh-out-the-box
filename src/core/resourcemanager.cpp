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

std::unique_ptr<StaticSprite> ResourceManager::loadSprite(const std::string &imagePath, bool alpha) {
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
        Logger::log("Texture loading error: " + imagePath);
        Logger::log("Reason: " + std::string(stbi_failure_reason()));
    }

    staticSpr->sprite->createSprite(width, height, data);
    stbi_image_free(data);

    return std::move(staticSpr);
}

// Animated Sprite

std::unique_ptr<AnimatedSprite> ResourceManager::loadSprite(const std::string &imagePath, const std::string &configPath, bool alpha) {
    YAML::Node sprConfig = YAML::LoadFile(configPath);

    YAML::Node anim = sprConfig["animation"];

    if (!anim.IsMap()) {
        Logger::log("Warning: Couldn't open animation config: " + configPath);
    }

    int rows = anim["rows"].as<int>(1);
    int columns = anim["columns"].as<int>(1);
    int frames = anim["frames"].as<int>(1);
    int fps = anim["fps"].as<int>(1);

    std::unique_ptr<AnimatedSprite> animSpr = std::make_unique<AnimatedSprite>(std::make_unique<Sprite>(), rows, columns, fps, frames);

    if (alpha) {
        animSpr->sprite->internal_format = GL_RGBA;
        animSpr->sprite->img_format = GL_RGBA;
    }

    int width;
    int height;
    int nrChannels;

    unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, alpha ? 4 : 3);
    if (!data) {
        Logger::log("Texture loading error: " + imagePath);
        Logger::log("Reason: " + std::string(stbi_failure_reason()));
    }

    animSpr->sprite->createSprite(width, height, data);
    stbi_image_free(data);

    return std::move(animSpr);
}

// Texture Atlas

std::unique_ptr<TextureAtlas> ResourceManager::loadAtlas(const std::string &imagePath, const std::string &configPath, bool alpha) {
    YAML::Node config = YAML::LoadFile(configPath);

    YAML::Node tilesetInfo = config["tileset"];
    if (!tilesetInfo.IsMap()) {
        Logger::log("Unable to load tileset info: " + configPath);
    }

    std::unique_ptr<TextureAtlas> atlas = std::make_unique<TextureAtlas>(std::make_unique<Sprite>(), static_cast<int>(tilesetInfo.size()));

    if (alpha) {
        atlas->sprite->internal_format = GL_RGBA;
        atlas->sprite->img_format = GL_RGBA;
    }

    int width;
    int height;
    int nrChannels;

    unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, alpha ? 4 : 3);
    if (!data) {
        Logger::log("Texture loading error: " + imagePath);
        Logger::log("Reason: " + std::string(stbi_failure_reason()));
    }

    atlas->sprite->createSprite(width, height, data);
    stbi_image_free(data);

    int gridsize = config["gridsize"].as<int>(32);

    if (width % gridsize != 0 || height % gridsize != 0) {
        Logger::log("Warning: Texture atlas does not have simetric grid, textures may be clipped");
        Logger::log("Config path: " + configPath);
        Logger::log("Image path: " + imagePath);
    }

    for (auto it : tilesetInfo) {
        std::string name = it.first.as<std::string>("");
        int x = it.second["x"].as<int>(0);
        int y = it.second["y"].as<int>(0);
        int width = it.second["width"].as<int>(1) * gridsize;
        int height = it.second["height"].as<int>(1) * gridsize;

        atlas->addEntry(name, x, y, width, height);
    }

    return std::move(atlas);
}

// Scene

std::unique_ptr<Scene> ResourceManager::loadScene(const std::string &scenePath, std::unique_ptr<TextureAtlas> sceneAtlas) {
    YAML::Node config = YAML::LoadFile(scenePath);

    std::string nullchar = config["nullchar"].as<std::string>("~");
    YAML::Node sceneInfo = config["sprites"];
    if (!sceneInfo.IsMap()) {
        Logger::log("Unable to load scene info: " + scenePath);
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

                while (sstream >> token) {  // reads each word separated by space
                    if (token != nullchar) {
                        
                        GameObject object;

                        object.setAtlas(scene->atlas.get());
                        object.atlasKey = token;
                        object.zIndex = zIndex;

                        object.size = glm::vec2(Game::objectSize);
                        object.position = glm::vec2(
                            Game::objectSize * x,
                            Game::objectSize * y);

                        Logger::log(token + " " + std::to_string(x) + " " + std::to_string(y));

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