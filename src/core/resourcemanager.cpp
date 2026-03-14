#include "../native/glglfw.h"
#include "../native/stb_image.h"

#include "yaml-cpp/yaml.h"
#include "util.hpp"

#include <fstream>
#include <sstream>

#include "resourcemanager.hpp"

Sprite ResourceManager::loadSprite(const std::string &imagePath, const std::string &configPath, bool alpha) {
    YAML::Node sprConfig = YAML::LoadFile(configPath);

    YAML::Node anim = sprConfig["animation"];

    if (!anim.IsMap()) {
        Logger::log("Warning: Couldn't open animation config: " + configPath);
    }

    int rows = anim["rows"].as<int>(1);
    int columns = anim["columns"].as<int>(1);
    int frames = anim["frames"].as<int>(1);
    int fps = anim["fps"].as<int>(1);

    SpriteAnimationInfo animInfo(rows, columns, fps, frames);

    Sprite sprite;
    if (alpha) { // TODO: Fix alpha (Some images work, some dont)
        sprite.internal_format = GL_RGBA;
        sprite.img_format = GL_RGBA;
    }

    int width;
    int height;
    int nrChannels;

    unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, alpha ? 4 : 3);
    if (!data) {
        Logger::log("Texture loading error: " + std::string(imagePath));
        Logger::log("Reason: " + std::string(stbi_failure_reason()));
    } 

    sprite.setAnimationInfo(animInfo);
    sprite.createSprite(width, height, data);
    stbi_image_free(data);

    return sprite;
}

Sprite ResourceManager::loadSprite(const std::string &imagePath, bool alpha) {
    SpriteAnimationInfo animInfo(1, 1, 1, 1);

    Sprite sprite;
    if (alpha) { // TODO: Fix alpha (Some images work, some dont)
        sprite.internal_format = GL_RGBA;
        sprite.img_format = GL_RGBA;
    }

    int width;
    int height;
    int nrChannels;

    unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, alpha ? 4 : 3);
    if (!data) {
        Logger::log("Texture loading error: " + std::string(imagePath));
        Logger::log("Reason: " + std::string(stbi_failure_reason()));
    } 

    sprite.setAnimationInfo(animInfo);
    sprite.createSprite(width, height, data);
    stbi_image_free(data);

    return sprite;
}

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