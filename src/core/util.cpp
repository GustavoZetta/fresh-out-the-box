
#include "util.hpp"

#include <iostream>
#include <algorithm>

std::string Common::getContentPath() {
    return "../../content";
}


std::string Common::toUpperCase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::string Common::toLowerCase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// TODO: more robust logging
void Logger::log(std::string message) {
    std::cout << message << std::endl;
}

