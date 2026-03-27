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
        Logger::warn("Invalid card value config");
    }

    return std::move(value);
}

std::unordered_map<std::string, Card> ResourceManager::loadCards(const std::string &configPath, TextureAtlas *cardAtlas) {

    std::unordered_map<std::string, Card> cardList;

    YAML::Node config = YAML::LoadFile(configPath);

    if (config.IsMap()) {
        for (auto cardConfig : config) {
            Card card;

            card.cardAtlas = cardAtlas;
            if (cardConfig.second.IsMap()) {
                card.id = cardConfig.first.as<std::string>("card");

                card.name = cardConfig.second["name"].as<std::string>("<name>");
                card.rarity = cardConfig.second["rarity"].as<std::string>("<rarity>");
                card.description = cardConfig.second["description"].as<std::vector<std::string>>(std::vector<std::string>{"<description>"});

                YAML::Node baseStats = cardConfig.second["base_stats"];

                if (baseStats.IsMap()) {
                    if (baseStats["attack"].IsDefined()) {
                        card.attack = std::move(ResourceManager::getCardValue(baseStats["attack"], CardValueType::ATTACK));
                    }
                    if (baseStats["stamina"].IsDefined()) {
                        card.stamina = std::move(ResourceManager::getCardValue(baseStats["stamina"], CardValueType::STAMINA));
                    }
                    if (baseStats["health"].IsDefined()) {
                        card.health = std::move(ResourceManager::getCardValue(baseStats["health"], CardValueType::HEALTH));
                    }
                    if (baseStats["defense"].IsDefined()) {
                        card.defense = std::move(ResourceManager::getCardValue(baseStats["defense"], CardValueType::DEFENSE));
                    }
                } else {
                    Logger::warn("Invalid base stats config: " + configPath);
                }

                YAML::Node cardType = cardConfig.second["card_type"];

                if (cardType.IsMap()) {
                    card.cardType = cardType["type"].as<std::string>("<type>");

                    if (cardType["defeats"].IsMap()) {
                        for (auto defeat : cardType["defeats"]) {
                            std::string key = defeat.first.as<std::string>();

                            card.defeats[key] = ResourceManager::getCardValue(defeat.second, CardValueType::ATTACK);
                        }
                    }
                } else {
                    Logger::warn("Invalid card type config: " + configPath);
                }

                YAML::Node cardColor = cardConfig.second["card_color"];

                if (cardColor.IsMap()) {
                    card.cardColor = cardColor["color"].as<std::string>("<color>");
                    std::array<int, 3> intcolor = cardColor["rgb"].as<std::array<int, 3>>(std::array<int, 3>{255, 255, 255});

                    card.color = glm::vec3((float)intcolor[0] / 255, (float)intcolor[1] / 255, (float)intcolor[2] / 255);

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
                    Logger::warn("Invalid card color config: " + configPath);
                }

            } else {
                Logger::warn("Invalid card config: " + configPath);
            }

            cardList[card.id] = std::move(card);
        }
    } else {
        Logger::error("Couldn't load card config: " + configPath);
    }
    return cardList;
}

std::unordered_map<std::string, CardPack> ResourceManager::loadCardPacks(const std::string &configPath) {
    std::unordered_map<std::string, CardPack> cardPackList;

    YAML::Node config = YAML::LoadFile(configPath);

    if (config.IsMap()) {
        for (auto cardPackConfig : config) {
            CardPack pack;

            if (cardPackConfig.second.IsMap()) {
                pack.id = cardPackConfig.first.as<std::string>("card");
                
                pack.name = cardPackConfig.second["name"].as<std::string>("<name>");
                pack.description = cardPackConfig.second["description"].as<std::vector<std::string>>(std::vector<std::string>{"<description>"});

                std::array<int, 3> intcolor = cardPackConfig.second["rgb"].as<std::array<int, 3>>(std::array<int, 3>{255, 255, 255});
                pack.color = glm::vec3((float)intcolor[0] / 255, (float)intcolor[1] / 255, (float)intcolor[2] / 255);

                YAML::Node cards = cardPackConfig.second["cards"];

                if (cards.IsSequence()) {
                    std::vector<CardWeight> cardsWeights;
                    int totalWeights = 0;

                    // Get total weights from all cards
                    for (auto card : cards) {
                        CardWeight weight;

                        weight.id = card["id"].as<std::string>("card");
                        weight.weight = card["weight"].as<int>(1);

                        totalWeights += weight.weight;
                        cardsWeights.push_back(std::move(weight));
                    }

                    // Calculate probabilities for all cards
                    for (CardWeight& weight : cardsWeights) {
                        pack.probabilities[weight.id] = (float) weight.weight / totalWeights;
                    }
                } else {
                    Logger::warn("Invalid cards in card pack config: " + configPath);
                }
            } else {
                Logger::warn("Invalid card pack config: " + configPath);
            }

            cardPackList[pack.id] = std::move(pack);
        }
    } else {
        Logger::error("Couldn't load card config: " + configPath);
    }

    return cardPackList;
}