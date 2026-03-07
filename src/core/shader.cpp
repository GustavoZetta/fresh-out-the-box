#include "../native/glglfw.h"

#include <array>
#include <fstream>
#include <sstream>
#include <string>

#include <glm/common.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "logger.hpp"

// L stands for local (as it is used only here in shader.cpp)
// SP stands for Shader Path (path being file path, not any path)
#define L_SP_VERTEX "../../src/shaders/vertex.glsl"
#define L_SP_FRAGMENT "../../src/shaders/fragment.glsl"

std::string Shader::getShaderSource(const std::string& filePath) {
    // Some black magic to get the file content
    std::ifstream file(filePath);
    std::stringstream buffer;

    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::createShaderModule(const char *shaderSrc, unsigned int shaderType) {
    unsigned int shaderP = glCreateShader(shaderType);

    glShaderSource(shaderP, 1, &shaderSrc, NULL);
    glCompileShader(shaderP);

    int sucess;
    glGetShaderiv(shaderP, GL_COMPILE_STATUS, &sucess);
    if (!sucess) {
        char errorLog[1024];
        glGetShaderInfoLog(shaderP, 1024, NULL, errorLog);
        Logger::log("Shader Module compilation error:\n");
        Logger::log(errorLog);
    }

    return shaderP;
}

unsigned int Shader::createShaderProgram() {
    std::array<unsigned int, 2> modules = {};

    { // RAII takes care of this
        std::string vertexSrc = getShaderSource(L_SP_VERTEX);
        std::string fragmentSrc = getShaderSource(L_SP_FRAGMENT);

        modules[0] = Shader::createShaderModule(vertexSrc.c_str(), GL_VERTEX_SHADER);
        modules[1] = Shader::createShaderModule(fragmentSrc.c_str(), GL_FRAGMENT_SHADER);
    }
    

    unsigned int shader = glCreateProgram();
    for (unsigned int shaderModule : modules) {
        glAttachShader(shader, shaderModule);
    }
    glLinkProgram(shader);

    int sucess;
    glGetProgramiv(shader, GL_LINK_STATUS, &sucess);
    if (!sucess) {
        char errorLog[1024];
        glGetProgramInfoLog(shader, 1024, NULL, errorLog);
        Logger::log("Shader Linking error:\n");
        Logger::log(errorLog);
    }

    // Cleaning modules cause it isn't need after the program creation
    for (unsigned int shaderModule : modules) {
        glDeleteShader(shaderModule);
    }

    return shader;
}

Shader::Shader() {
    m_id = createShaderProgram();
};

Shader::~Shader() {
    glDeleteProgram(m_id);
}

unsigned int Shader::ID() {
    return m_id;
}

void Shader::setMat4x4(const char *uniformName, const glm::mat4 &matrix, bool useProgram) {
    if (useProgram) {
        glUseProgram(m_id);
    }

    glUniformMatrix4fv(glGetUniformLocation(m_id, uniformName), 1, false, glm::value_ptr(matrix));
}

// GLM vec

void Shader::setVec4f(const char *uniformName, const glm::vec4 &vector, bool useProgram) {
    if (useProgram) {
        glUseProgram(m_id);
    }

    glUniform4f(glGetUniformLocation(m_id, uniformName), vector.x, vector.y, vector.z, vector.w);
}

void Shader::setVec3f(const char *uniformName, const glm::vec3 &vector, bool useProgram) {
    if (useProgram) {
        glUseProgram(m_id);
    }

    glUniform3f(glGetUniformLocation(m_id, uniformName), vector.x, vector.y, vector.z);
}

void Shader::setVec2f(const char *uniformName, const glm::vec2 &vector, bool useProgram) {
    if (useProgram) {
        glUseProgram(m_id);
    }

    glUniform2f(glGetUniformLocation(m_id, uniformName), vector.x, vector.y);
}

// Raw Data

void Shader::setVec4f(const char *uniformName, float x, float y, float z, float w, bool useProgram) {
    if (useProgram) {
        glUseProgram(m_id);
    }

    glUniform4f(glGetUniformLocation(m_id, uniformName), x, y, z, w);
}

void Shader::setVec3f(const char *uniformName, float x, float y, float z, bool useProgram) {
    if (useProgram) {
        glUseProgram(m_id);
    }

    glUniform3f(glGetUniformLocation(m_id, uniformName), x, y, z);
}

void Shader::setVec2f(const char *uniformName, float x, float y, bool useProgram) {
    if (useProgram) {
        glUseProgram(m_id);
    }

    glUniform2f(glGetUniformLocation(m_id, uniformName), x, y);
}
