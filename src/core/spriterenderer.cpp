#include "../native/glglfw.h"

#include "logger.hpp"
#include "spriterenderer.hpp"

#include <memory>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

SpriteRenderer::SpriteRenderer() : shader(nullptr), spriteList({}) {
    spriteList.reserve(16);
}

void SpriteRenderer::init() {
    Logger::log("Initializing Sprite Renderer...");
    shader = std::make_unique<Shader>();
}

void SpriteRenderer::render() {
    glUseProgram(shader->ID());

    for (Sprite &spr : spriteList) {
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(spr.position, 0.0f));
        model = glm::translate(model, glm::vec3(0.5 * spr.size.x, 0.5 * spr.size.y, 0.0));

        model = glm::rotate(model, glm::radians(spr.rotation), glm::vec3(0.0, 0.0, 1.0));

        model = glm::translate(model, glm::vec3(-0.5 * spr.size.x, -0.5 * spr.size.y, 0.0));
        model = glm::scale(model, glm::vec3(spr.size, 1.0f));

        shader->setMat4x4("model", model, false);
        shader->setVec3f("spriteColor", spr.color, false);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, spr.texture->ID());

        glBindVertexArray(spr.getVAO());
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}

void SpriteRenderer::cleanup() {
    spriteList = {}; // Sprite destructor deletes VAOs & VBOs
    shader = nullptr; // Shader destructor deletes shader program
}

void SpriteRenderer::createSprite() {
    unsigned int VAO;
    unsigned int VBO;
    float vertices[] = {
        // pos // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Sprite sprite(VAO, VBO);
    sprite.texture = std::make_unique<Texture>();
}