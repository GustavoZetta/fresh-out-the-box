#pragma once

class Texture {
  public:
    // Texture Image Info
    int width, height;
    unsigned int img_format; // Image format is RGB (Basically no reason to change this)
    // Filtering Modes
    unsigned int min_filter_mode; // if texture is smaller than the size requested to be rendered
    unsigned int max_filter_mode; // if texture is bigger than the size requested to be rendered
    // Wrapping texture mode (Like GL_REPEAT)
    unsigned int wrap_mode_s;
    unsigned int wrap_mode_t;

  public:
    Texture();
    void create(unsigned int width, unsigned int height, unsigned char *data);

    // Returns the OpenGL Texture ID
    unsigned int ID();

  private:
    // Texture ID
    unsigned int m_id;
};