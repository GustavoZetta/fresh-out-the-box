#include "core/menu.hpp"

Menu::Menu(std::unique_ptr<TextureAtlas> atlas) {
    menuAtlas = std::move(atlas);
}