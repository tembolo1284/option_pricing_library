// include/server/Server.hpp
#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <memory>

class Server {
public:
    Server(const std::string& address, int port);
    void start();
    void stop();
    std::string getStatus() const;

private:
    std::string serverAddress;
    int serverPort;
    bool isRunning;

    void handleRequest(const std::string& request);
};

#endif // SERVER_HPP

