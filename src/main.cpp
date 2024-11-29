// src/main.cpp
#include "server/Server.hpp"
#include "utils/Logger.hpp"

int main() {
    Logger::log("Starting Pricing Server...", Logger::Level::INFO);

    Server server("localhost", 50051);
    server.start();

    // Simulate server running
    Logger::log("Server is running. Press Ctrl+C to stop.", Logger::Level::INFO);

    // Placeholder for actual server lifecycle
    while (true) {}

    server.stop();
    Logger::log("Server stopped.", Logger::Level::INFO);

    return 0;
}

