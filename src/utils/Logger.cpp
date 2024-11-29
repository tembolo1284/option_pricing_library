// src/utils/Logger.cpp
#include "utils/Logger.hpp"

void Logger::log(const std::string& message, Level level) {
    std::string prefix;
    switch (level) {
        case Level::INFO: prefix = "[INFO] "; break;
        case Level::WARN: prefix = "[WARN] "; break;
        case Level::ERROR: prefix = "[ERROR] "; break;
    }
    std::cout << prefix << message << std::endl;
}

