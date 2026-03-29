#pragma once

#include <glm/common.hpp>

#include <vector>

#include "core/renderer/particle/particle.hpp"

struct EmitterConfig {
    glm::vec2 position;
    glm::vec2 velocityMin;
    glm::vec2 velocityMax;
    glm::vec2 acceleration;
    glm::vec4 colorStart;
    glm::vec4 colorEnd;
    float lifetimeMin;
    float lifetimeMax;
    float sizeStart;
    float sizeEnd;
    int emitRate;
};

class ParticleEmitter {
  public:
    EmitterConfig config;
    bool active = true;

  private:
    std::vector<Particle> m_particles;
    float m_emitTimer = 0.0f;

  public:
    ParticleEmitter(EmitterConfig config, int maxParticles);

    void update(float deltaTime);

    std::vector<Particle>* getParticles();

  private:
    void emit();

    Particle createParticle();
};