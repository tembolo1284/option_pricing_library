// src/models/BermudanOption.cpp
#include "models/BermudanOption.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

BermudanOption::BermudanOption(double spot, double strike, double rate, 
                             double volatility, double time, int numExerciseDates)
    : spotPrice(spot), strikePrice(strike), riskFreeRate(rate),
      volatility(volatility), timeToMaturity(time),
      numExerciseDates(numExerciseDates) {}

double BermudanOption::calculatePrice() const {
    return backwardInduction();
}

std::string BermudanOption::getModelName() const {
    return "Bermudan Option";
}

double BermudanOption::backwardInduction() const {
    // Number of time steps in the binomial tree
    // We'll use more steps than exercise dates for better accuracy
    const int numSteps = std::max(200, 4 * numExerciseDates);
    const double dt = timeToMaturity / numSteps;
    
    // Calculate up and down factors and probabilities
    const double u = std::exp(volatility * std::sqrt(dt));
    const double d = 1.0 / u;
    const double discountFactor = std::exp(-riskFreeRate * dt);
    const double p = (std::exp(riskFreeRate * dt) - d) / (u - d);
    
    // Create vectors to store option values at each node
    std::vector<double> values(numSteps + 1);
    std::vector<double> newValues(numSteps + 1);
    
    // Initialize terminal values
    for (int i = 0; i <= numSteps; ++i) {
        double spotAtNode = spotPrice * std::pow(u, numSteps - i) * std::pow(d, i);
        values[i] = std::max(0.0, spotAtNode - strikePrice);
    }
    
    // Exercise dates (excluding maturity)
    std::vector<int> exerciseDates;
    if (numExerciseDates > 1) {
        double dtExercise = timeToMaturity / (numExerciseDates - 1);
        for (int i = numExerciseDates - 2; i >= 0; --i) {
            int timeStep = static_cast<int>(std::round(i * dtExercise / dt));
            if (timeStep > 0) {  // Ensure we don't include t=0
                exerciseDates.push_back(timeStep);
            }
        }
    }
    
    // Backward induction
    for (int step = numSteps - 1; step >= 0; --step) {
        for (int i = 0; i <= step; ++i) {
            double spotAtNode = spotPrice * std::pow(u, step - i) * std::pow(d, i);
            
            // Calculate continuation value
            double continuationValue = discountFactor * 
                (p * values[i] + (1.0 - p) * values[i + 1]);
            
            // Check if this is an exercise date
            bool isExerciseDate = std::find(exerciseDates.begin(), 
                                          exerciseDates.end(), 
                                          step) != exerciseDates.end();
            
            if (isExerciseDate) {
                // Compare continuation value with immediate exercise value
                double exerciseValue = std::max(0.0, spotAtNode - strikePrice);
                newValues[i] = std::max(continuationValue, exerciseValue);
            } else {
                newValues[i] = continuationValue;
            }
        }
        
        // Swap vectors for next iteration
        values.swap(newValues);
    }
    
    // Return option price at t=0
    return values[0];
}
