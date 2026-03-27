#pragma once

#include <glm/common.hpp>

#include <string>
#include <unordered_map>

struct CardWeight {
    std::string id;
    int weight;
};

class CardPack {
  public:
    std::string id;

    std::string name;
    std::vector<std::string> description;
    glm::vec3 color;

    std::unordered_map<std::string, float> probabilities; // <card_id, probability>
};