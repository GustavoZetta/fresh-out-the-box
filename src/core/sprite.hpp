
#include "texture.hpp"
#include <glm/common.hpp>
#include <memory>

class Sprite {
  public:
    // Just stores the VAO and VBO, then when the Sprite gets destroyed, it deletes both
    Sprite(unsigned int VAO, unsigned int VBO); 
    // Deletes VAO & VBO
    ~Sprite();

    unsigned int getVAO();
    unsigned int getVBO();

  public:
    std::unique_ptr<Texture> texture;

    glm::vec3 color;

    glm::vec2 position;
    glm::vec2 size;
    float rotation;

  private:
    unsigned int m_VAO;
    unsigned int m_VBO;
};