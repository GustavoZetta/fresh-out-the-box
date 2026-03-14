#include "util.hpp"

#include "sprite.hpp"

// Anim Info

SpriteAnimationInfo::SpriteAnimationInfo(int rows, int cols, int fps, int frames)
    : cols(cols), rows(rows), totalFrames(frames),
      currentFrame(0), frameDuration(1.0f / fps), timer(0.0f) {}

void SpriteAnimationInfo::update(float deltaTime) {
    timer += deltaTime;
    if (timer >= frameDuration) {
        timer = 0.0f;
        currentFrame = (currentFrame + 1) % totalFrames;
    }
}

glm::vec2 SpriteAnimationInfo::getUVOffset() const {
    int col = currentFrame % cols;
    int row = currentFrame / cols;
    return glm::vec2(
        (float)col / cols,
        (float)row / rows);
}

glm::vec2 SpriteAnimationInfo::getUVSize() const {
    return glm::vec2(1.0f / cols, 1.0f / rows);
}

// Sprite

Sprite::Sprite() : m_id(0), animInfo(nullptr), width(0), height(0), internal_format(GL_RGB), img_format(GL_RGB), min_filter_mode(GL_NEAREST), max_filter_mode(GL_NEAREST), wrap_mode_s(GL_REPEAT), wrap_mode_t(GL_REPEAT) {}

Sprite::~Sprite() {
    glDeleteTextures(1, &m_id);
}

void Sprite::setAnimationInfo(SpriteAnimationInfo anim) {
    animInfo = std::make_unique<SpriteAnimationInfo>(std::move(anim));
}

void Sprite::createSprite(int w, int h, unsigned char *data) {
    if (animInfo == nullptr) {
        Logger::log("Warning: Created a sprite with no SpriteAnimationInfo");
    }

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

void Sprite::update(float deltaTime) {
    if (animInfo != nullptr) {
        animInfo->update(deltaTime);
    }
}

unsigned int Sprite::ID() {
    return m_id;
}
