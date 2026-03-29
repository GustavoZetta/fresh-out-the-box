#include "core/util.hpp"

#include "core/renderer/particle/particleemitter.hpp"

ParticleEmitter::ParticleEmitter(EmitterConfig config, int maxParticles) : config(std::move(config)) {
    m_particles.reserve(maxParticles);
}

void ParticleEmitter::update(float deltaTime) {
    if (active) {
        m_emitTimer += deltaTime;
        float emitInterval = 1.0f / config.emitRate;

        while (m_emitTimer >= emitInterval) {
            m_emitTimer -= emitInterval;
            emit();
        }
    }

    for (Particle &p : m_particles) {
        if (!p.isAlive())
            continue;

        p.remainingLife -= deltaTime;
        p.position += p.velocity * deltaTime;
        p.velocity += config.acceleration * deltaTime;

        float t = p.lifeRatio();
        p.color = glm::mix(config.colorEnd, config.colorStart, t);

        p.size = glm::mix(config.sizeEnd, config.sizeStart, t);
    }
}

std::vector<Particle> *ParticleEmitter::getParticles() {
    return &m_particles;
}

void ParticleEmitter::emit() {
    for (auto &p : m_particles) {
        if (!p.isAlive()) {
            p = createParticle();
            return;
        }
    }

    m_particles.emplace_back(createParticle());
}

Particle ParticleEmitter::createParticle() {
    Particle p;
    p.position = config.position;
    p.velocity = glm::vec2(
        Common::randomFloat(config.velocityMin.x, config.velocityMax.x),
        Common::randomFloat(config.velocityMin.y, config.velocityMax.y)
    );
    p.lifetime = Common::randomFloat(config.lifetimeMin, config.lifetimeMax);
    p.remainingLife = p.lifetime;
    p.color = config.colorStart;
    p.size = config.sizeStart;
    return p;
}