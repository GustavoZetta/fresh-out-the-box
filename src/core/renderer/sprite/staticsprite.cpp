
#include "core/renderer/sprite/staticsprite.hpp"

StaticSprite::StaticSprite(std::unique_ptr<Sprite> spr) : uvOffset(0.0f), uvSize(1.0f) {
    sprite = std::move(spr);
}
