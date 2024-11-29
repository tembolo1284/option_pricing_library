// test/models/LeastSquaresMonteCarloTest.cpp
#include "models/LeastSquaresMonteCarlo.hpp"
#include <gtest/gtest.h>

TEST(LeastSquaresMonteCarloTest, CalculatePrice) {
    LeastSquaresMonteCarlo model(100.0, 100.0, 0.05, 0.2, 1.0, 10000, 10);
    double price = model.calculatePrice();
    EXPECT_NEAR(price, 9.8, 0.5); // Placeholder for expected price
}

TEST(LeastSquaresMonteCarloTest, GetModelName) {
    LeastSquaresMonteCarlo model(100.0, 100.0, 0.05, 0.2, 1.0, 10000, 10);
    EXPECT_EQ(model.getModelName(), "Least Squares Monte Carlo");
}

