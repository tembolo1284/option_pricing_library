// test/models/BinomialTreeTest.cpp
#include "models/BinomialTree.hpp"
#include <gtest/gtest.h>

TEST(BinomialTreeTest, CalculatePriceEuropean) {
    BinomialTree model(100.0, 100.0, 0.05, 0.2, 1.0, 50, false);
    double price = model.calculatePrice();
    EXPECT_NEAR(price, 10.35, 0.01); // Expected price for sample inputs
}

TEST(BinomialTreeTest, CalculatePriceAmerican) {
    BinomialTree model(100.0, 100.0, 0.05, 0.2, 1.0, 50, true);
    double price = model.calculatePrice();
    EXPECT_NEAR(price, 10.75, 0.01); // Expected price for sample inputs
}

TEST(BinomialTreeTest, GetModelName) {
    BinomialTree model(100.0, 100.0, 0.05, 0.2, 1.0, 50, true);
    EXPECT_EQ(model.getModelName(), "Binomial Tree");
}

