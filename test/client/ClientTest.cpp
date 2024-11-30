// test/client/ClientTest.cpp
#include "Client.hpp"
#include <gtest/gtest.h>

TEST(ClientTest, SendRequest) {
    Client client("localhost", 50051);
    ASSERT_NO_THROW(client.sendRequest("Test Request"));
}

TEST(ClientTest, ReceiveResponse) {
    Client client("localhost", 50051);
    std::string response = client.receiveResponse();
    EXPECT_EQ(response, "Server Response");
}

