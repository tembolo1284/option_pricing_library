// client/include/Client.hpp
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
public:
    Client(const std::string& serverAddress, int serverPort);
    ~Client();

    // Prevent copying
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    void sendRequest(const std::string& request);
    std::string receiveResponse();

private:
    std::string serverAddress;
    int serverPort;
    int socketFd;  // Socket file descriptor

    bool connectToServer();
    void cleanup();
};

#endif // CLIENT_HPP
