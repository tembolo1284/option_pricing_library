// client/src/Client.cpp
#include "Client.hpp"
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

Client::Client(const std::string& serverAddress, int serverPort) 
    : serverAddress(serverAddress), serverPort(serverPort), socketFd(-1) {
    spdlog::debug("Creating client for {}:{}", serverAddress, serverPort);
}

void Client::sendRequest(const std::string& request) {
    spdlog::debug("Preparing to send request: {}", request);
    
    try {
        if (!connectToServer()) {
            throw std::runtime_error("Failed to connect to server");
        }

        // Send message length first (for message framing)
        uint32_t messageLength = static_cast<uint32_t>(request.length());
        messageLength = htonl(messageLength); // Convert to network byte order
        
        if (send(socketFd, &messageLength, sizeof(messageLength), 0) != sizeof(messageLength)) {
            throw std::runtime_error("Failed to send message length");
        }

        // Send the actual message
        ssize_t bytesSent = send(socketFd, request.c_str(), request.length(), 0);
        if (bytesSent != static_cast<ssize_t>(request.length())) {
            throw std::runtime_error("Failed to send complete message");
        }

        spdlog::info("Successfully sent request of {} bytes", bytesSent);

    } catch (const std::exception& e) {
        spdlog::error("Error sending request: {}", e.what());
        cleanup();
        throw;
    }
}

std::string Client::receiveResponse() {
    spdlog::debug("Waiting to receive response");
    
    try {
        if (socketFd == -1) {
            throw std::runtime_error("Not connected to server");
        }

        // First receive the message length
        uint32_t messageLength;
        ssize_t bytesReceived = recv(socketFd, &messageLength, sizeof(messageLength), 0);
        
        if (bytesReceived != sizeof(messageLength)) {
            throw std::runtime_error("Failed to receive message length");
        }
        
        messageLength = ntohl(messageLength); // Convert from network byte order
        
        // Now receive the actual message
        std::string response;
        response.resize(messageLength);
        size_t totalReceived = 0;
        
        while (totalReceived < messageLength) {
            bytesReceived = recv(socketFd, &response[totalReceived], 
                               messageLength - totalReceived, 0);
            
            if (bytesReceived <= 0) {
                throw std::runtime_error("Connection closed by server");
            }
            
            totalReceived += bytesReceived;
        }

        spdlog::info("Successfully received response of {} bytes", totalReceived);
        cleanup(); // Close the connection after receiving response
        return response;

    } catch (const std::exception& e) {
        spdlog::error("Error receiving response: {}", e.what());
        cleanup();
        throw;
    }
}

bool Client::connectToServer() {
    spdlog::debug("Attempting to connect to {}:{}", serverAddress, serverPort);
    
    // Create socket
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1) {
        spdlog::error("Failed to create socket: {}", strerror(errno));
        return false;
    }

    // Set up server address structure
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    
    if (inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr) <= 0) {
        spdlog::error("Invalid address: {}", serverAddress);
        cleanup();
        return false;
    }

    // Connect to server
    if (connect(socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        spdlog::error("Connection failed: {}", strerror(errno));
        cleanup();
        return false;
    }

    // Set timeout for socket operations
    struct timeval timeout;
    timeout.tv_sec = 5;  // 5 seconds timeout
    timeout.tv_usec = 0;

    if (setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        spdlog::warn("Failed to set receive timeout");
    }

    if (setsockopt(socketFd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        spdlog::warn("Failed to set send timeout");
    }

    spdlog::info("Successfully connected to server");
    return true;
}

void Client::cleanup() {
    if (socketFd != -1) {
        close(socketFd);
        socketFd = -1;
        spdlog::debug("Closed socket connection");
    }
}

Client::~Client() {
    cleanup();
}
