#pragma once

#include "core/renderer/animatedsprite.hpp"
#include "core/renderer/shader.hpp"
#include "core/renderer/staticsprite.hpp"
#include "core/renderer/textureatlas.hpp"
#include "core/scene.hpp"

namespace ResourceManager {
    std::unique_ptr<StaticSprite> loadSprite(const std::string &imagePath, bool alpha);
    std::unique_ptr<AnimatedSprite> loadSprite(const std::string &imagePath, const std::string &configPath, bool alpha);
    std::unique_ptr<TextureAtlas> loadAtlas(const std::string &imagePath, const std::string &configPath, bool alpha);
    std::unique_ptr<Scene> loadScene(const std::string &scenePath, std::unique_ptr<TextureAtlas> sceneAtlas);

    Shader loadShader(const std::string &vertexPath, const std::string &fragmentPath);

    std::string getTextFromFile(const std::string &vertexPath);
};