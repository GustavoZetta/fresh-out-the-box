#include "core/util.hpp"

#include <algorithm>

#include "core/renderer/particle/particleemitter.hpp"

ParticleEmitter::ParticleEmitter(EmitterConfig config, int maxParticles) : type(EmitterType::DEFAULT), config(std::move(config)) {
    m_particles.reserve(maxParticles);
}

void ParticleEmitter::setAtlas(TextureAtlas* a) {
    type = EmitterType::SPRITE;
    atlas = a;
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

    std::sort(m_particles.begin(), m_particles.end(), [](const Particle& a, const Particle& b) {
        return a.remainingLife < b.remainingLife;
    });
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
    p.position = config.position + glm::vec2(
        Common::randomFloat(-config.spawnRange.x, config.spawnRange.x),
        Common::randomFloat(-config.spawnRange.y, config.spawnRange.y)
    );
    
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