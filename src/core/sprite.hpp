#pragma once
#include "../native/glglfw.h"

#include <memory>
#include <string>

#include <glm/common.hpp>

class SpriteAnimationInfo {
  public:
    int totalFrames;
    int currentFrame;
    int cols;
    int rows;
    float frameDuration;
    float timer;

  public:
    SpriteAnimationInfo(int cols, int rows, float frameDuration);

    void update(float deltaTime);

    glm::vec2 getUVOffset() const;
    glm::vec2 getUVSize() const;
};

class Sprite {
  public:
    std::unique_ptr<SpriteAnimationInfo> animInfo;

    // Texture Image Info
    int width;
    int height;

    unsigned int img_format;      // The format of the texture that OpenGL will store as
    unsigned int internal_format; // Format of the image data used in createTexture()
    // Filtering Modes
    unsigned int min_filter_mode; // if texture is smaller than the size requested to be rendered
    unsigned int max_filter_mode; // if texture is bigger than the size requested to be rendered
    // Wrapping texture mode (Like GL_REPEAT)
    unsigned int wrap_mode_s;
    unsigned int wrap_mode_t;

  public:
    // Gets its content from a file
    Sprite();
    ~Sprite();

    Sprite(Sprite &&other) noexcept;

    Sprite &operator=(Sprite &&other) noexcept {
        if (this != &other) {
            glDeleteTextures(1, &m_id);
            m_id = other.m_id;
            animInfo = std::move(other.animInfo);
            other.m_id = 0;
            width = other.width;
            height = other.height;
            img_format = other.img_format;
            internal_format = other.internal_format;
            min_filter_mode = other.min_filter_mode;
            max_filter_mode = other.max_filter_mode;
            wrap_mode_s = other.wrap_mode_s;
            wrap_mode_t = other.wrap_mode_t;
        }
        return *this;
    }

    void Sprite::setAnimationInfo(SpriteAnimationInfo animInfo);

    // Creates the Open GL texture
    void createSprite(int width, int height, unsigned char *data);

    // Returns the OpenGL Texture ID
    unsigned int ID();

  private:
    // Texture ID
    unsigned int m_id;
};