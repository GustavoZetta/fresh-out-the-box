#include "core/renderer/particle/particle.hpp"

bool Particle::isAlive() {
    return remainingLife > 0.0f;
}

float Particle::lifeRatio() {
    return remainingLife / lifetime;
}