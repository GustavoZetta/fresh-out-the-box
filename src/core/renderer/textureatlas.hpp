#pragma once

#include <glm/common.hpp>

#include <memory>
#include <string>
#include <unordered_map>

#include "core/renderer/sprite.hpp"

struct AtlasEntry {
    glm::vec2 uvOffset;
    glm::vec2 uvSize;
};

// Expects you to initialize the sprite
class TextureAtlas {
  public:
    std::unique_ptr<Sprite> sprite;
    
  public:
    TextureAtlas(std::unique_ptr<Sprite> spr);
    TextureAtlas(std::unique_ptr<Sprite> spr, int amountToAlloc);

    // Registers an atlas entry and put it into entries
    void addEntry(const std::string &name, int x, int y, int w, int h);

    AtlasEntry getEntry(const std::string &name);

  private:
    std::unordered_map<std::string, AtlasEntry> m_entries;
};