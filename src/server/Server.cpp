// src/server/Server.cpp
#include "server/Server.hpp"
#include <iostream>
#include <stdexcept>

Server::Server(const std::string& address, int port)
    : serverAddress(address), serverPort(port), isRunning(false) {}

void Server::start() {
    isRunning = true;
    std::cout << "Server started at " << serverAddress << ":" << serverPort << std::endl;
    // Placeholder: Implement server logic
}

void Server::stop() {
    isRunning = false;
    std::cout << "Server stopped." << std::endl;
}

std::string Server::getStatus() const {
    return isRunning ? "Running" : "Stopped";
}

void Server::handleRequest(const std::string& request) {
    // Placeholder: Implement request handling
    std::cout << "Received request: " << request << std::endl;
}

