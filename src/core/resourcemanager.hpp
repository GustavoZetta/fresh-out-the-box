#pragma once

#include "core/renderer/shader.hpp"

#include "core/renderer/sprite/animatedsprite.hpp"
#include "core/renderer/sprite/staticsprite.hpp"
#include "core/renderer/sprite/textureatlas.hpp"

#include "core/scene.hpp"

namespace ResourceManager {
    std::unique_ptr<StaticSprite> loadStaticSprite(const std::string &imagePath, bool alpha);
    std::unique_ptr<AnimatedSprite> loadAnimatedSprite(const std::string &configPath, bool alpha);
    std::unique_ptr<TextureAtlas> loadTextureAtlas(const std::string &configPath, bool alpha);
    std::unique_ptr<Scene> loadScene(const std::string &scenePath, std::unique_ptr<TextureAtlas> sceneAtlas);

    Shader loadShader(const std::string &vertexPath, const std::string &fragmentPath);

    std::string getTextFromFile(const std::string &vertexPath);
}; // namespace ResourceManager