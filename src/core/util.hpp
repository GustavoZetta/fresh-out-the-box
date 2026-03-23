#pragma once

#include <string>

struct Common {
    static std::string getContentPath();
    static std::string toUpperCase(std::string str);
    static std::string toLowerCase(std::string str);
};

// TODO: more robust logging
struct Logger {
    static void log(std::string message);
};