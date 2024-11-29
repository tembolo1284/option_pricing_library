// src/models/LeastSquaresMonteCarlo.cpp
#include "models/LeastSquaresMonteCarlo.hpp"
#include <vector>
#include <cmath>
#include <random>
#include <numeric>

LeastSquaresMonteCarlo::LeastSquaresMonteCarlo(double spot, double strike, double rate, double volatility, double time, int numPaths, int numExerciseDates)
    : spotPrice(spot), strikePrice(strike), riskFreeRate(rate), volatility(volatility), timeToMaturity(time),
      numPaths(numPaths), numExerciseDates(numExerciseDates) {}

double LeastSquaresMonteCarlo::calculatePrice() const {
    auto paths = generatePaths();
    return regressionAndDiscounting(paths);
}

std::string LeastSquaresMonteCarlo::getModelName() const {
    return "Least Squares Monte Carlo";
}

std::vector<std::vector<double>> LeastSquaresMonteCarlo::generatePaths() const {
    std::vector<std::vector<double>> paths(numPaths, std::vector<double>(numExerciseDates));
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    for (int i = 0; i < numPaths; ++i) {
        double currentPrice = spotPrice;
        for (int j = 0; j < numExerciseDates; ++j) {
            double dt = timeToMaturity / numExerciseDates;
            double dS = currentPrice * (riskFreeRate * dt + volatility * sqrt(dt) * distribution(generator));
            currentPrice += dS;
            paths[i][j] = currentPrice;
        }
    }
    return paths;
}

double LeastSquaresMonteCarlo::regressionAndDiscounting(const std::vector<std::vector<double>>& paths) const {
    // Placeholder: Implement regression for optimal exercise decisions
    return 0.0;
}

