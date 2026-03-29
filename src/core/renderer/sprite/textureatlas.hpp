#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "core/renderer/sprite/uvinfo.hpp"

#include "core/renderer/sprite/sprite.hpp"

// Expects you to initialize the sprite
class TextureAtlas {
  public:
    std::unique_ptr<Sprite> sprite;
    
  public:
    TextureAtlas(std::unique_ptr<Sprite> spr);
    TextureAtlas(std::unique_ptr<Sprite> spr, int amountToAlloc);

    // Calculates an UvInfo based on params and adds it into the sprite map
    void addUvInfo(const std::string &name, int x, int y, int w, int h);

    UvInfo getUvInfo(const std::string &name);

  private:
    // Holds the information of all sprites
    std::unordered_map<std::string, UvInfo> m_spriteInfoMap;
};