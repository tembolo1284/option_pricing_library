// src/utils/MathUtils.cpp
#include "utils/MathUtils.hpp"
#include <cmath>
#include <stdexcept>

namespace MathUtils {
    double factorial(int n) {
        if (n < 0) throw std::invalid_argument("Negative factorial not defined");
        return n <= 1 ? 1 : n * factorial(n - 1);
    }

    double power(double base, int exp) {
        return std::pow(base, exp);
    }

    double combination(int n, int k) {
        if (k < 0 || k > n) throw std::invalid_argument("Invalid combination indices");
        return factorial(n) / (factorial(k) * factorial(n - k));
    }

    double normalCDF(double x) {
        return 0.5 * erfc(-x * M_SQRT1_2);
    }

    double normalPDF(double x) {
        return exp(-0.5 * x * x) / sqrt(2 * M_PI);
    }
}

