#pragma once

#include <string>

namespace Common {
    std::string getContentPath();
    std::string getContentPath(const std::string &path);
    std::string toUpperCase(std::string str);
    std::string toLowerCase(std::string str);

    float randomFloat(float min, float max);
}; // namespace Common

// TODO: more robust logging
namespace Logger {
    static const bool canLog = true;
    static const bool canInfo = true;
    static const bool canWarn = true;
    static const bool canError = true;

    void log(std::string message);
    void info(std::string message);
    void warn(std::string message);
    void error(std::string message);
}; // namespace Logger