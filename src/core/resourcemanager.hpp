#pragma once

#include "shader.hpp"
#include "sprite.hpp"

class ResourceManager {
  public:
    static Sprite loadSprite(const std::string &filePath, bool alpha);
    static Shader loadShader(const std::string &vertexPath, const std::string &fragmentPath);

  private:
    static std::string getTextFromFile(const std::string &vertexPath);
};