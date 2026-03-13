#pragma once

#define GLFW_INCLUDE_GLCOREARB

#include <glm/common.hpp>
#include <memory>

#include "gameobject.hpp"
#include "shader.hpp"
#include "sprite.hpp"

class SpriteRenderer {
  public:
    std::unique_ptr<Shader> shader;

  public:
    SpriteRenderer();

    void createScreen(); // Creates the VBO & VAO used to render
    void drawSprite(Sprite &sprite, glm::vec3 color, glm::vec2 position, glm::vec2 size, float rotation); // Draws a sprite with just raw data
    void drawSprite(GameObject *obj); // Draws the data from a game object

    // Inits with a shader created by the ResourceManager
    void init(Shader s);
    void cleanup();

  private:
    unsigned int m_VAO;
    unsigned int m_VBO;
};