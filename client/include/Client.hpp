// client/include/Client.hpp
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
public:
    Client(const std::string& serverAddress, int serverPort);
    void sendRequest(const std::string& request);
    std::string receiveResponse();

private:
    std::string serverAddress;
    int serverPort;
};

#endif // CLIENT_HPP
