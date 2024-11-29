// src/models/MonteCarlo.cpp
#include "models/MonteCarlo.hpp"
#include <cmath>
#include <random>

MonteCarlo::MonteCarlo(double spot, double strike, double rate, double volatility, double time, int numPaths, bool isAsian)
    : spotPrice(spot), strikePrice(strike), riskFreeRate(rate), volatility(volatility), timeToMaturity(time),
      numPaths(numPaths), asianOption(isAsian) {}

double MonteCarlo::calculatePrice() const {
    return asianOption ? calculateAsianPrice() : 0.0; // Placeholder
}

std::string MonteCarlo::getModelName() const {
    return "Monte Carlo";
}

double MonteCarlo::calculateAsianPrice() const {
    double sumPayoffs = 0.0;
    for (int i = 0; i < numPaths; ++i) {
        auto path = generatePricePath();
        double avgPrice = std::accumulate(path.begin(), path.end(), 0.0) / path.size();
        sumPayoffs += std::max(avgPrice - strikePrice, 0.0);
    }
    return exp(-riskFreeRate * timeToMaturity) * (sumPayoffs / numPaths);
}

std::vector<double> MonteCarlo::generatePricePath() const {
    std::vector<double> path;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    double currentPrice = spotPrice;
    for (int i = 0; i < 100; ++i) { // Placeholder for time steps
        double dt = timeToMaturity / 100;
        double dS = currentPrice * (riskFreeRate * dt + volatility * sqrt(dt) * distribution(generator));
        currentPrice += dS;
        path.push_back(currentPrice);
    }
    return path;
}

