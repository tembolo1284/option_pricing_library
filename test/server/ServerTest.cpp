// test/server/ServerTest.cpp
#include "server/Server.hpp"
#include <gtest/gtest.h>

TEST(ServerTest, StartAndStop) {
    Server server("localhost", 50051);
    server.start();
    EXPECT_EQ(server.getStatus(), "Running");
    server.stop();
    EXPECT_EQ(server.getStatus(), "Stopped");
}

TEST(ServerTest, HandleRequest) {
    Server server("localhost", 50051);
    server.start();
    ASSERT_NO_THROW(server.handleRequest("Price Option"));
}

