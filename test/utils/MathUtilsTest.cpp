// test/utils/MathUtilsTest.cpp
#include "utils/MathUtils.hpp"
#include <gtest/gtest.h>

TEST(MathUtilsTest, Factorial) {
    EXPECT_EQ(MathUtils::factorial(5), 120);
    EXPECT_EQ(MathUtils::factorial(0), 1);
}

TEST(MathUtilsTest, Combination) {
    EXPECT_EQ(MathUtils::combination(5, 2), 10);
}

TEST(MathUtilsTest, NormalCDF) {
    EXPECT_NEAR(MathUtils::normalCDF(0.0), 0.5, 0.01);
}

