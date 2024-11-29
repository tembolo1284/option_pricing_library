// client/src/Client.cpp
#include "client/Client.hpp"
#include <iostream> // Placeholder for actual network libraries

Client::Client(const std::string& serverAddress, int serverPort)
    : serverAddress(serverAddress), serverPort(serverPort) {}

void Client::sendRequest(const std::string& request) {
    std::cout << "Sending request to " << serverAddress << ":" << serverPort << " - " << request << std::endl;
    // Placeholder for actual send logic
}

std::string Client::receiveResponse() {
    std::cout << "Receiving response from server..." << std::endl;
    // Placeholder for actual receive logic
    return "Server Response";
}

