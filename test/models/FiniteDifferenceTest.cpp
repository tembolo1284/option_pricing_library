// test/models/FiniteDifferenceTest.cpp
#include "models/FiniteDifference.hpp"
#include <gtest/gtest.h>

TEST(FiniteDifferenceTest, CalculatePriceExplicit) {
    FiniteDifference model(100.0, 100.0, 0.05, 0.2, 1.0, 100, FiniteDifference::Scheme::Explicit);
    double price = model.calculatePrice();
    EXPECT_NEAR(price, 10.4, 0.1);
}

TEST(FiniteDifferenceTest, GetModelName) {
    FiniteDifference model(100.0, 100.0, 0.05, 0.2, 1.0, 100, FiniteDifference::Scheme::CrankNicolson);
    EXPECT_EQ(model.getModelName(), "Finite Difference - Crank-Nicolson");
}

