// test/server/ServerTest.cpp
#include <gtest/gtest.h>
#include "server/Server.hpp"

class ServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        spdlog::set_level(spdlog::level::debug);
    }
};

TEST_F(ServerTest, Constructor_ValidParameters) {
    ASSERT_NO_THROW(Server server("localhost", 8080));
}

TEST_F(ServerTest, StartStop_Sequence) {
    Server server("localhost", 8080);
    ASSERT_NO_THROW(server.start());
    ASSERT_EQ(server.getStatus(), "Running");
    ASSERT_NO_THROW(server.stop());
    ASSERT_EQ(server.getStatus(), "Stopped");
}

TEST_F(ServerTest, HandleRequest_ServerRunning) {
    Server server("localhost", 8080);
    server.start();
    ASSERT_NO_THROW(server.handleRequest("Price Option"));
}

TEST_F(ServerTest, HandleRequest_ServerStopped) {
    Server server("localhost", 8080);
    ASSERT_THROW(server.handleRequest("Price Option"), std::runtime_error);
}
