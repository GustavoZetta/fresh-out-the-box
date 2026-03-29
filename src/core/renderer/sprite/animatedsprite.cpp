#include "core/util.hpp"

#include "core/renderer/sprite/animatedsprite.hpp"

AnimatedSprite::AnimatedSprite(std::unique_ptr<Sprite> spr)
    : m_currentFrame(0), m_timer(0.0f), m_frameDuration(0.0f) {
    sprite = std::move(spr);
}

AnimatedSprite::AnimatedSprite(std::unique_ptr<Sprite> spr, int amountToAlloc) : m_currentFrame(0), m_timer(0.0f), m_frameDuration(0.0f) {
    sprite = std::move(spr);
}

void AnimatedSprite::update(float deltaTime) {
    m_timer += deltaTime;
    if (m_timer >= m_frameDuration) {
        int remove = m_timer / m_frameDuration; // Skip frames if the fps is too low
        m_timer -= m_frameDuration * remove;

        m_currentFrame = (m_currentFrame + remove) % m_frames;
    }
}

void AnimatedSprite::setState(const std::string &s) {
    state = s;
    m_frames = static_cast<int>(m_animationInfoMap.at(state).animationFrameInfo.size());
    m_frameDuration = 1.0f / m_animationInfoMap.at(state).fps;
    m_currentFrame = 0;
}

UvInfo AnimatedSprite::calcUvInfo(int x, int y, int w, int h) {
    UvInfo uv;
    uv.uvOffset = glm::vec2(
        (float)x / sprite->width,
        (float)y / sprite->height);
    uv.uvSize = glm::vec2(
        (float)w / sprite->width,
        (float)h / sprite->height);
    return uv;
}

void AnimatedSprite::addAnimationInfoList(const std::string &state, AnimatedInfoList infoList) {
    m_animationInfoMap[state] = std::move(infoList);
    if (m_frameDuration == 0.0f) {
        setState(state);
    }
}

UvInfo AnimatedSprite::getUvInfo() {
    return m_animationInfoMap.at(state).animationFrameInfo.at(m_currentFrame);
}