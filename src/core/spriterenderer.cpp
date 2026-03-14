#include "../native/glglfw.h"

#include "spriterenderer.hpp"
#include "util.hpp"

#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <memory>

SpriteRenderer::SpriteRenderer() : shader(nullptr) {}

void SpriteRenderer::createScreen() {
    float vertices[] = {
        // pos // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f};

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

void SpriteRenderer::drawSprite(Sprite &sprite, glm::vec3 color, glm::vec2 position, glm::vec2 size, float rotation) {
    glUseProgram(shader->ID());

    // Position
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.0));

    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));

    model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.0));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader->setMat4x4("model", model, false);
    shader->setVec3f("spriteColor", color, false);

    // Animation
    glm::vec2 uvOffset = glm::vec2(0.0f);
    glm::vec2 uvSize = glm::vec2(1.0f);

    uvOffset = sprite.animInfo->getUVOffset();
    uvSize = sprite.animInfo->getUVSize();

    shader->setVec2f("uvOffset", uvOffset, false);
    shader->setVec2f("uvSize", uvSize, false);

    // Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sprite.ID());

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::drawSprite(GameObject *obj) {
    glUseProgram(shader->ID());

    // Position
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(obj->position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5 * obj->size.x, 0.5 * obj->size.y, 0.0));

    model = glm::rotate(model, glm::radians(obj->rotation), glm::vec3(0.0, 0.0, 1.0));

    model = glm::translate(model, glm::vec3(-0.5 * obj->size.x, -0.5 * obj->size.y, 0.0));
    model = glm::scale(model, glm::vec3(obj->size, 1.0f));

    shader->setMat4x4("model", model, false);
    shader->setVec3f("spriteColor", obj->color, false);

    // Animation
    glm::vec2 uvOffset = glm::vec2(0.0f);
    glm::vec2 uvSize = glm::vec2(1.0f);

    uvOffset = obj->sprite->animInfo->getUVOffset();
    uvSize = obj->sprite->animInfo->getUVSize();

    shader->setVec2f("uvOffset", uvOffset, false);
    shader->setVec2f("uvSize", uvSize, false);

    // Textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obj->sprite->ID());

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

// Init / Exit logic

void SpriteRenderer::init(Shader s) {
    Logger::log("Initializing Sprite Renderer...");
    shader = std::make_unique<Shader>(std::move(s));

    shader->setInt("image", 0, true);

    createScreen();
}

void SpriteRenderer::cleanup() {
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
    shader.reset(); // Shader destructor deletes shader program
}
