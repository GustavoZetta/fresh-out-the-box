#pragma once

#define GLFW_INCLUDE_GLCOREARB

#include "native/glglfw.h"

#include <glm/common.hpp>
#include <memory>

#include "core/gameobject.hpp"
#include "core/renderer/particle/particleemitter.hpp"
#include "core/renderer/shader.hpp"
#include "core/renderer/sprite/sprite.hpp"

class SpriteRenderer {
  public:
    std::unique_ptr<Shader> shader;

  public:
    SpriteRenderer();

    void createScreen();              // Creates the VBO & VAO used to render
    void drawObject(GameObject *obj); // Draws to the screen the data from a game object

    void drawParticles(ParticleEmitter *particleEmitter); // Draws to the screen the data from the particles

    // Inits with a shader created by the ResourceManager
    void init(Shader s);
    void cleanup();

  private:
    unsigned int m_VAO;
    unsigned int m_VBO;
};