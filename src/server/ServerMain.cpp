// src/server/ServerMain.cpp
#include "server/Server.hpp"
#include <spdlog/spdlog.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        // Set up logging
        spdlog::set_level(spdlog::level::debug);
        spdlog::info("Starting pricing server...");

        // Default values
        std::string address = "127.0.0.1";
        int port = 8080;

        // Parse command line arguments if provided
        if (argc == 3) {
            address = argv[1];
            port = std::stoi(argv[2]);
        }

        // Create and start server
        Server server(address, port);
        server.start();

        spdlog::info("Server running on {}:{}", address, port);
        spdlog::info("Press Enter to stop the server...");
        
        // Wait for user input to stop
        std::cin.get();
        
        server.stop();
        spdlog::info("Server stopped");
        
        return 0;
    } catch (const std::exception& e) {
        spdlog::error("Server error: {}", e.what());
        return 1;
    }
}
