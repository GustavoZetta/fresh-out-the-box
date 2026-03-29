
#include "util.hpp"

#include <algorithm>
#include <iostream>

std::string Common::getContentPath() {
    return "../../content";
}

std::string Common::getContentPath(const std::string &s) {
    return "../../content" + s;
}

std::string Common::toUpperCase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::string Common::toLowerCase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

float Common::randomFloat(float min, float max) {
    return min + (float)rand() / RAND_MAX * (max - min);
}

// TODO: more robust logging
void Logger::log(std::string message) {
    if (Logger::canLog) {
        std::cout << message << std::endl;
    }
}

void Logger::info(std::string message) {
    if (Logger::canInfo) {
        std::cout << "[INFO] " << message << std::endl;
    }
}

void Logger::warn(std::string message) {
    if (Logger::canWarn) {
        std::cout << "[WARN] " << message << std::endl;
    }
}

void Logger::error(std::string message) {
    if (Logger::canError) {
        std::cout << "[ERROR] " << message << std::endl;
    }
}
