// src/server/Server.cpp
#include "server/Server.hpp"
#include <stdexcept>

Server::Server(const std::string& address, int port) 
    : serverAddress(address), serverPort(port), isRunning(false) {
    spdlog::debug("Server initializing with address: {} and port: {}", address, port);
    validateServerConfig();
}

void Server::start() {
    if (isRunning) {
        spdlog::warn("Attempt to start already running server at {}:{}", serverAddress, serverPort);
        return;
    }
    
    try {
        initializeServer();
        isRunning = true;
        spdlog::info("Server successfully started at {}:{}", serverAddress, serverPort);
    } catch (const std::exception& e) {
        spdlog::error("Failed to start server: {}", e.what());
        throw;
    }
}

void Server::stop() {
    if (!isRunning) {
        spdlog::warn("Attempt to stop already stopped server at {}:{}", serverAddress, serverPort);
        return;
    }
    
    isRunning = false;
    spdlog::info("Server stopped at {}:{}", serverAddress, serverPort);
}

std::string Server::getStatus() const {
    spdlog::debug("Status request for server at {}:{}", serverAddress, serverPort);
    return isRunning ? "Running" : "Stopped";
}

void Server::handleRequest(const std::string& request) {
    if (!isRunning) {
        spdlog::error("Attempt to handle request while server is not running");
        throw std::runtime_error("Server is not running");
    }
    
    spdlog::info("Handling request: {}", request);
    // Add request handling logic here
}

void Server::initializeServer() {
    spdlog::debug("Initializing server components");
    // Add initialization logic here
}

void Server::validateServerConfig() const {
    spdlog::debug("Validating server configuration");
    
    if (serverPort <= 0 || serverPort > 65535) {
        spdlog::error("Invalid port number: {}", serverPort);
        throw std::invalid_argument("Invalid port number");
    }
    
    if (serverAddress.empty()) {
        spdlog::error("Empty server address");
        throw std::invalid_argument("Server address cannot be empty");
    }
}
