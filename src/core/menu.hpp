#pragma once

#include <glm/common.hpp>

#include <string>
#include <functional>

#include "core/gameobject.hpp"

class MenuButton : public GameObject {
public:
    std::string label;
    std::function<void()> action;
};

class Menu {
public:
    std::vector<MenuButton> buttons;
};