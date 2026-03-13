#include "../native/glglfw.h"
#include "../native/stb_image.h"

#include "yaml-cpp/yaml.h"
#include "util.hpp"

#include <fstream>
#include <sstream>

#include "resourcemanager.hpp"

Sprite ResourceManager::loadSprite(const std::string &filePath, bool alpha) {
    Sprite sprite;
    if (alpha) { // TODO: Fix alpha (Some images work, some dont)
        sprite.internal_format = GL_RGBA;
        sprite.img_format = GL_RGBA;
    }

    int width;
    int height;
    int nrChannels;

    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, alpha ? 4 : 3);
    if (!data) {
        Logger::log("Texture loading error: " + std::string(filePath));
        Logger::log("Reason: " + std::string(stbi_failure_reason()));
    } 

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