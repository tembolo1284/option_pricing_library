// src/models/BlackScholes.cpp
#include "models/BlackScholes.hpp"
#include <cmath>
#include <stdexcept>

BlackScholes::BlackScholes(double spot, double strike, double rate, double volatility, double time)
    : spotPrice(spot), strikePrice(strike), riskFreeRate(rate), volatility(volatility), timeToMaturity(time) {}

double BlackScholes::calculatePrice() const {
    double d1 = calculateD1();
    double d2 = calculateD2();
    double price = spotPrice * normalCDF(d1) - strikePrice * exp(-riskFreeRate * timeToMaturity) * normalCDF(d2);
    return price;
}

std::string BlackScholes::getModelName() const {
    return "Black-Scholes";
}

double BlackScholes::calculateD1() const {
    return (log(spotPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) /
           (volatility * sqrt(timeToMaturity));
}

double BlackScholes::calculateD2() const {
    return calculateD1() - volatility * sqrt(timeToMaturity);
}

double BlackScholes::normalCDF(double value) const {
    return 0.5 * erfc(-value * M_SQRT1_2);
}

