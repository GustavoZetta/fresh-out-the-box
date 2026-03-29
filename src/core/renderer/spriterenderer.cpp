#include "native/glglfw.h"

#include "core/util.hpp"

#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include <memory>

#include "core/renderer/spriterenderer.hpp"

SpriteRenderer::SpriteRenderer() : shader(nullptr) {}

void SpriteRenderer::createScreen() {
    float vertices[] = {
        // pos // tex
        0.0f,
        1.0f,
        0.0f,
        1.0f, // 1
        1.0f,
        0.0f,
        1.0f,
        0.0f, // 2
        0.0f,
        0.0f,
        0.0f,
        0.0f, // 3
        0.0f,
        1.0f,
        0.0f,
        1.0f, // 4
        1.0f,
        1.0f,
        1.0f,
        1.0f, // 5
        1.0f,
        0.0f,
        1.0f,
        0.0f // 6
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::drawObject(GameObject *obj) {
    glUseProgram(shader->ID());

    // Position
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(obj->position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5 * obj->size.x, 0.5 * obj->size.y, 0.0));

    model = glm::rotate(model, glm::radians(obj->rotation), glm::vec3(0.0, 0.0, 1.0));

    model = glm::translate(model, glm::vec3(-0.5 * obj->size.x, -0.5 * obj->size.y, 0.0));
    model = glm::scale(model, glm::vec3(obj->size, 1.0f));

    shader->setMat4x4("model", model, false);
    shader->setVec4f("spriteColor", obj->color, false);

    shader->setInt("useTexture", 1, false);

    // Animation
    glm::vec2 uvOffset = glm::vec2(0.0f);
    glm::vec2 uvSize = glm::vec2(1.0f);

    {
        UvInfo uv;

        switch (obj->sprType) {
        case SpriteType::STATIC_SPRITE:
            uvOffset = obj->staticSprite->uvOffset;
            uvSize = obj->staticSprite->uvSize;
            break;
        case SpriteType::ANIMATED_SPRITE:
            uv = obj->animatedSprite->getUvInfo();
            uvOffset = uv.uvOffset;
            uvSize = uv.uvSize;
            break;
        case SpriteType::TEXTURE_ATLAS:
            uv = obj->textureAtlas->getUvInfo(obj->atlasKey);
            uvOffset = uv.uvOffset;
            uvSize = uv.uvSize;
            break;
        }
    }

    shader->setVec2f("uvOffset", uvOffset, false);
    shader->setVec2f("uvSize", uvSize, false);

    // Textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obj->getSpriteID());

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::drawParticles(ParticleEmitter *particleEmitter) {
    glUseProgram(shader->ID());

    UvInfo uv;

    if (particleEmitter->type == EmitterType::SPRITE) {
        shader->setInt("useTexture", 1, false);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, particleEmitter->atlas->sprite->ID());

        uv = particleEmitter->atlas->getUvInfo(particleEmitter->atlasKey);
    } else {
        shader->setVec2f("uvOffset", 0.0f, false);
        shader->setVec2f("uvSize", 1.0f, false);

        shader->setInt("useTexture", 0, false);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    for (Particle &p : *particleEmitter->getParticles()) {
        if (!p.isAlive())
            continue;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(p.position, 0.0f));
        model = glm::scale(model, glm::vec3(p.size, p.size, 1.0f));

        shader->setMat4x4("model", model, false);
        shader->setVec4f("spriteColor", p.color, false);

        if (particleEmitter->type == EmitterType::SPRITE) {
            shader->setVec2f("uvOffset", uv.uvOffset, false);
            shader->setVec2f("uvSize", uv.uvSize, false);
        }

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}

// Init / Exit logic

void SpriteRenderer::init(Shader s) {
    Logger::info("Initializing Sprite Renderer...");
    shader = std::make_unique<Shader>(std::move(s));

    shader->setInt("image", 0, true);

    createScreen();
}

void SpriteRenderer::cleanup() {
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
    shader.reset(); // Shader destructor deletes shader program
}
