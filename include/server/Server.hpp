// include/server/Server.hpp
#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <memory>
#include <spdlog/spdlog.h>

class Server {
public:
    Server(const std::string& address, int port);
    void start();
    void stop();
    std::string getStatus() const;

    void handleRequest(const std::string& request);
private:
    std::string serverAddress;
    int serverPort;
    bool isRunning;

    void initializeServer();
    void validateServerConfig() const;

};

#endif // SERVER_HPP

