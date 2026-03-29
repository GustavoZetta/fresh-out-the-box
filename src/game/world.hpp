#pragma once

#include <vector>

#include "core/gameobject.hpp"
#include "core/scene.hpp"

#include "game/card/card.hpp"
#include "game/card/cardpack.hpp"

#include "core/renderer/particle/particleemitter.hpp"

#include "game/player.hpp"

class World {
  public:
    std::unique_ptr<Player> player;
    std::unique_ptr<GameObject> bg;
    std::unique_ptr<Scene> scene;
    std::vector<GameObject> objects;

    std::vector<ParticleEmitter> emitters;

    std::unique_ptr<TextureAtlas> cardAtlas;
    std::unordered_map<std::string, Card> cards;
    std::unordered_map<std::string, CardPack> cardPacks;

  public:
    World();

    void update(float deltaTime);

    std::string rollCardPack(std::string cardPackId); // Returns the id of the card gotten
};