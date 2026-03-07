#pragma once

#include <string>

class Shader {
  public:

    // The shader gets created as soon the constructor is called,
    // so it needs the correct OpenGL context.
    // Without correct context, as being something 
    // like "static Shader coolshader()",
    // expect errors
    Shader();

    // Deletes the shader program
    ~Shader();

    // Returns the shader program ID
    unsigned int ID();

    void setMat4x4(const char *uniformName, const glm::mat4 &matrix, bool useProgram);

    void setVec4f(const char *uniformName, const glm::vec4 &vector, bool useProgram);
    void setVec3f(const char *uniformName, const glm::vec3 &vector, bool useProgram);
    void setVec2f(const char *uniformName, const glm::vec2 &vector, bool useProgram);

    void setVec4f(const char *uniformName, float x, float y, float z, float w, bool useProgram);
    void setVec3f(const char *uniformName, float x, float y, float z, bool useProgram);
    void setVec2f(const char *uniformName, float x, float y, bool useProgram);

  private:
    // Puts the content of a file into a string and returns it (It doesn't care if it is shader code or not lol)
    static std::string getShaderSource(const std::string& filePath);
    // Creates a compiled shader code and returns the ID which can be put into a shader program
    static unsigned int createShaderModule(const char *shaderSrc, unsigned int shaderType);
    // Creates the shader program and returns the ID
    static unsigned int createShaderProgram();

    // Shader program ID
    unsigned int m_id;
};