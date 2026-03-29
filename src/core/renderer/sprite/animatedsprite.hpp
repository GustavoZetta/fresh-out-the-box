#pragma once

#include <glm/common.hpp>

#include <memory>
#include <string>
#include <unordered_map>

#include "core/renderer/sprite/uvinfo.hpp"

#include "core/renderer/sprite/sprite.hpp"

struct AnimatedInfoList {
    std::vector<UvInfo> animationFrameInfo;
    int fps;
};

// Expects you to initialize the sprite
class AnimatedSprite {
  public:
    std::unique_ptr<Sprite> sprite;
    std::string state;

  public:
    AnimatedSprite(std::unique_ptr<Sprite> spr);
    AnimatedSprite(std::unique_ptr<Sprite> spr, int amountToAlloc);

    void update(float deltaTime);
    // Sets the state, resets the frame counter and gets the amount of frames of the new state
    void setState(const std::string &state);

    // Calculates the UvInfo using the sprite object inside the AnimatedSprite
    UvInfo calcUvInfo(int x, int y, int w, int h);

    // Adds a list of "animation inforsmation" to the list of states
    // (Don't keep using the vector if its a variable, it gets moved into the state map)
    void addAnimationInfoList(const std::string &state, AnimatedInfoList infoList);
    UvInfo getUvInfo();

  private:
    // States (or ID) that points a list of UvInfo
    // Think of the vector as a list of "animation information" that is used to get the actual images for each frame in the sprite
    std::unordered_map<std::string, AnimatedInfoList> m_animationInfoMap;
    int m_frames;

    float m_frameDuration;
    int m_currentFrame;
    float m_timer;
};