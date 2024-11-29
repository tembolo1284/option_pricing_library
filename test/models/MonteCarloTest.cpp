// test/models/MonteCarloTest.cpp
#include "models/MonteCarlo.hpp"
#include <gtest/gtest.h>

TEST(MonteCarloTest, CalculateAsianPrice) {
    MonteCarlo model(100.0, 100.0, 0.05, 0.2, 1.0, 10000, true);
    double price = model.calculatePrice();
    EXPECT_NEAR(price, 9.45, 0.5); // Monte Carlo has higher variance
}

TEST(MonteCarloTest, GetModelName) {
    MonteCarlo model(100.0, 100.0, 0.05, 0.2, 1.0, 10000, true);
    EXPECT_EQ(model.getModelName(), "Monte Carlo");
}

