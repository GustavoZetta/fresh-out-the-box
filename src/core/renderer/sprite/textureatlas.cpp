#include "core/renderer/sprite/textureatlas.hpp"

TextureAtlas::TextureAtlas(std::unique_ptr<Sprite> spr) {
    sprite = std::move(spr);
}

TextureAtlas::TextureAtlas(std::unique_ptr<Sprite> spr, int amountToAlloc) {
    sprite = std::move(spr);
    m_spriteInfoMap.reserve(amountToAlloc);
}

void TextureAtlas::addUvInfo(const std::string &name, int x, int y, int w, int h) {
    UvInfo entry;
    entry.uvOffset = glm::vec2(
        (float)x / sprite->width,
        (float)y / sprite->height);
    entry.uvSize = glm::vec2(
        (float)w / sprite->width,
        (float)h / sprite->height);
    m_spriteInfoMap[name] = entry;
}

UvInfo TextureAtlas::getUvInfo(const std::string &name) {
    return m_spriteInfoMap.at(name);
}