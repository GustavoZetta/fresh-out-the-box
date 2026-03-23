#pragma once

#include <yaml-cpp/yaml.h>

#include "game/card/card.hpp"

#include "core/resourcemanager.hpp"

namespace ResourceManager {
    CardValue getCardValue(YAML::Node &valueConfig, CardValueType type);
    std::vector<Card> loadCardConfig(const std::string &configPath, TextureAtlas* cardAtlas);
}