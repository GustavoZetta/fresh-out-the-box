#pragma once

#include <glm/common.hpp>

#include <string>
#include <functional>

#include "core/gameobject.hpp"

class MenuButton : public GameObject {
public:
    std::string label;
    std::function<void()> enterMouse;
    std::function<void()> exitMouse;
    std::function<void()> action;
};

class Menu {
public:
    Menu(std::unique_ptr<TextureAtlas> atlas);

    std::vector<MenuButton> buttons;
    std::unique_ptr<TextureAtlas> menuAtlas;
};