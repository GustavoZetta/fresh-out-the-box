#include "core/util.hpp"

#include "core/renderer/sprite/sprite.hpp"

Sprite::Sprite() : m_id(0), width(0), height(0), internal_format(GL_RGB), img_format(GL_RGB), min_filter_mode(GL_NEAREST), max_filter_mode(GL_NEAREST), wrap_mode_s(GL_REPEAT), wrap_mode_t(GL_REPEAT) {}
Sprite::~Sprite() {
    glDeleteTextures(1, &m_id);
}

void Sprite::createSprite(int w, int h, unsigned char *data) {
    width = w;
    height = h;

    glGenTextures(1, &m_id);

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, img_format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_filter_mode);

    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Sprite::ID() {
    return m_id;
}
