
#include "logger.hpp"

#include <iostream>

// TODO: more robust logging
void Logger::log(std::string message) {
    if (true) { // Set false for a release build
        std::cout << message << std::endl;
    }
}