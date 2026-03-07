
#include "sprite.hpp"

#include "../native/glglfw.h"

Sprite::Sprite(unsigned int VAO, unsigned int VBO) : m_VAO(VAO), m_VBO(VBO) {}

Sprite::~Sprite() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

unsigned int Sprite::getVAO() {
    return m_VAO;
}

unsigned int Sprite::getVBO() {
    return m_VBO;
}