// include/utils/Logger.hpp
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>

class Logger {
public:
    enum class Level { INFO, WARN, ERROR };

    static void log(const std::string& message, Level level = Level::INFO);
};

#endif // LOGGER_HPP

