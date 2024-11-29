// src/models/BermudanOption.cpp
#include "models/BermudanOption.hpp"
#include <vector>
#include <cmath>

BermudanOption::BermudanOption(double spot, double strike, double rate, double volatility, double time, int numExerciseDates)
    : spotPrice(spot), strikePrice(strike), riskFreeRate(rate), volatility(volatility), timeToMaturity(time),
      numExerciseDates(numExerciseDates) {}

double BermudanOption::calculatePrice() const {
    return backwardInduction();
}

std::string BermudanOption::getModelName() const {
    return "Bermudan Option";
}

double BermudanOption::backwardInduction() const {
    // Placeholder: Implement backward induction logic for Bermudan options
    return 0.0;
}

