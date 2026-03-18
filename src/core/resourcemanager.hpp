#pragma once

#include "core/renderer/animatedsprite.hpp"
#include "core/renderer/shader.hpp"
#include "core/renderer/staticsprite.hpp"
#include "core/renderer/textureatlas.hpp"
#include "core/scene.hpp"

class ResourceManager {
  public:
    static std::unique_ptr<StaticSprite> loadSprite(const std::string &imagePath, bool alpha);
    static std::unique_ptr<AnimatedSprite> loadSprite(const std::string &imagePath, const std::string &configPath, bool alpha);
    static std::unique_ptr<TextureAtlas> loadAtlas(const std::string &imagePath, const std::string &configPath, bool alpha);
    static std::unique_ptr<Scene> loadScene(const std::string &scenePath, std::unique_ptr<TextureAtlas> sceneAtlas);

    static Shader loadShader(const std::string &vertexPath, const std::string &fragmentPath);

  private:
    static std::string getTextFromFile(const std::string &vertexPath);
};