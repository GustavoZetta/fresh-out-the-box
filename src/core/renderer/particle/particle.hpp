#pragma once

#include <glm/common.hpp>

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 color;

    float lifetime;
    float remainingLife;
    float size;

    bool isAlive();
    float lifeRatio();
};