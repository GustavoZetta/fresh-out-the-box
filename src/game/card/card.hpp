#pragma once

#include <glm/common.hpp>

#include "core/renderer/textureatlas.hpp"

enum class CardValueType {
    BASE, // Basically null card value type
    ATTACK,
    STAMINA,
    HEALTH,
    DEFENSE
};

namespace CardInfo {
    std::string cardValueTypeAsString(CardValueType type);
    CardValueType cardValueTypeFromString(const std::string &type);
}

struct CardValue {
    float value = 0.0f;
    float multiplier = 1.0f;
    float maxValue = 0.0f;
    float maxValueMultiplier = 1.0f;

    CardValueType type = CardValueType::BASE;
};

class Card {
  public:
    std::string id; // is also the atlas key

    std::string name;
    std::vector<std::string> description;

    TextureAtlas *cardAtlas;

    CardValue attack;
    CardValue stamina;
    CardValue health;
    CardValue defense;

    std::string cardType;
    // What type does this card defeats, or is superior (kinda how fire is destroyed by water)
    std::unordered_map<std::string, CardValue> defeats; // <Name of the type that it defeats, attack buffs values>

    std::string cardColor; // Name of the color
    glm::vec3 color;       // Actual color (This is aplied to the card border)
    // What buffs does the card have with other colors
    std::unordered_map<std::string, std::vector<CardValue>> buffs; // <Name of the colors that it buffs, list of buffs to them>
};