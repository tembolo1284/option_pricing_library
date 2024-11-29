// test/models/BlackScholesTest.cpp
#include "models/BlackScholes.hpp"
#include <gtest/gtest.h>

TEST(BlackScholesTest, CalculatePrice) {
    BlackScholes model(100.0, 100.0, 0.05, 0.2, 1.0);
    double price = model.calculatePrice();
    EXPECT_NEAR(price, 10.45, 0.01); // Expected price for sample inputs
}

TEST(BlackScholesTest, GetModelName) {
    BlackScholes model(100.0, 100.0, 0.05, 0.2, 1.0);
    EXPECT_EQ(model.getModelName(), "Black-Scholes");
}

