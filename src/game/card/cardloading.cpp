#include "core/util.hpp"

#include "game/card/cardloading.hpp"

CardValue ResourceManager::getCardValue(YAML::Node &valueConfig, CardValueType type) {
    CardValue value;

    value.type = type;

    if (valueConfig.IsMap()) {
        std::string typeName = Common::toLowerCase(CardInfo::cardValueTypeAsString(type));

        if (valueConfig[typeName + "_value"].IsDefined()) {
            value.value = valueConfig[typeName + "_value"].as<float>(0.0f);
        } else {
            value.value = valueConfig["value"].as<float>(0.0f);
        }

        if (valueConfig[typeName + "_multiplier"].IsDefined()) {
            value.multiplier = valueConfig[typeName + "_multiplier"].as<float>(1.0f);
        } else {
            value.multiplier = valueConfig["multiplier"].as<float>(1.0f);
        }

        if (valueConfig["max_value"].IsDefined()) {
            value.maxValue = valueConfig["max_value"].as<float>(0.0f);
        } else {
            value.maxValue = valueConfig["max_value_add"].as<float>(0.0f);
        }

        value.maxValueMultiplier = valueConfig["max_value_multiplier"].as<float>(1.0f);
    } else {
        Logger::log("Invalid card value config");
    }

    return value;
}

std::vector<Card> ResourceManager::loadCardConfig(const std::string &configPath, TextureAtlas *cardAtlas) {
    YAML::Node config = YAML::LoadFile(configPath);

    std::vector<Card> cardList;

    if (config.IsMap()) {
        for (auto cardConfig : config) {
            Card card;

            card.cardAtlas = cardAtlas;
            if (cardConfig.IsMap()) {
                card.id = cardConfig.first.as<std::string>("card");

                card.name = cardConfig.second["name"].as<std::string>("<name>");
                card.description = cardConfig.second["description"].as<std::vector<std::string>>(std::vector<std::string>{"<description>"});

                YAML::Node baseStats = cardConfig["base_stats"];

                if (baseStats.IsMap()) {
                    if (baseStats["attack"].IsDefined()) {
                        card.attack = ResourceManager::getCardValue(baseStats["attack"], CardValueType::ATTACK);
                    }
                    if (baseStats["stamina"].IsDefined()) {
                        card.stamina = ResourceManager::getCardValue(baseStats["stamina"], CardValueType::STAMINA);
                    }
                    if (baseStats["health"].IsDefined()) {
                        card.health = ResourceManager::getCardValue(baseStats["health"], CardValueType::HEALTH);
                    }
                    if (baseStats["defense"].IsDefined()) {
                        card.defense = ResourceManager::getCardValue(baseStats["defense"], CardValueType::DEFENSE);
                    }
                } else {
                    Logger::log("Invalid base stats config: " + configPath);
                }

                YAML::Node cardType = cardConfig["card_type"];

                if (cardType.IsMap()) {
                    card.cardType = cardType["type"].as<std::string>("<type>");

                    if (cardType["defeats"].IsMap()) {
                        for (auto defeat : cardType["defeats"]) {
                            std::string key = defeat.first.as<std::string>();
                            
                            card.defeats[key] = ResourceManager::getCardValue(defeat.second, CardValueType::ATTACK);
                        }
                    }
                } else {
                    Logger::log("Invalid card type config: " + configPath);
                }

                YAML::Node cardColor = cardConfig["card_color"];

                if (cardColor.IsMap()) {
                    card.cardColor = cardColor["color"].as<std::string>("<color>");
                    std::array<int, 3> intcolor = cardColor["rgb"].as<std::array<int, 3>>();

                    card.color = glm::vec3((float) intcolor[1] / 255, (float) intcolor[2] / 255, (float) intcolor[3] / 255);
                    
                    if (cardColor["buffs"].IsMap()) {
                        for (auto buff : cardColor["buffs"]) {
                            std::string key = buff.first.as<std::string>("<key>");

                            std::vector<CardValue> buffsValues;

                            if (buff.second.IsMap()) {
                                for (auto otherColor : buff.second) {
                                    CardValueType t = CardInfo::cardValueTypeFromString(otherColor.first.as<std::string>("BASE"));

                                    CardValue v = ResourceManager::getCardValue(otherColor.second, t);

                                    buffsValues.push_back(std::move(v));
                                }
                            }

                            card.buffs[key] = buffsValues;
                        }
                    }
                } else {
                    Logger::log("Invalid card color config: " + configPath);
                }
                
            } else {
                Logger::log("Invalid card config: " + configPath);
            }

            cardList.push_back(std::move(card));
        }
    } else {
        Logger::log("ERROR: couldn't load card config: " + configPath);
    }

    return cardList;
}