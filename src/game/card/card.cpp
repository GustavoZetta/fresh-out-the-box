#include "core/util.hpp"

#include "game/card/card.hpp"

std::string CardInfo::cardValueTypeAsString(CardValueType type) {
    switch (type) {
    case CardValueType::ATTACK:
        return "ATTACK";
    case CardValueType::STAMINA:
        return "STAMINA";
    case CardValueType::HEALTH:
        return "HEALTH";
    case CardValueType::DEFENSE:
        return "DEFENSE";
    }

    return "BASE";
}

CardValueType CardInfo::cardValueTypeFromString(const std::string &type) {
    std::string str = Common::toUpperCase(type);

    if (str == "ATTACK") {
        return CardValueType::ATTACK;
    } else if (str == "STAMINA") {
        return CardValueType::STAMINA;
    } else if (str == "HEALTH") {
        return CardValueType::HEALTH;
    } else if (str == "DEFENSE") {
        return CardValueType::DEFENSE;
    }

    return CardValueType::BASE;
}