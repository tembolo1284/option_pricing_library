// src/models/LeastSquaresMonteCarlo.cpp
// src/models/LeastSquaresMonteCarlo.cpp
#include "models/LeastSquaresMonteCarlo.hpp"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <random>
#include <spdlog/spdlog.h>

LeastSquaresMonteCarlo::LeastSquaresMonteCarlo(double spot, double strike, 
                                              double rate, double volatility, 
                                              double time, int paths, 
                                              int exerciseDates)
    : spotPrice(spot), strikePrice(strike), riskFreeRate(rate),
      volatility(volatility), timeToMaturity(time), numPaths(paths),
      numExerciseDates(exerciseDates) {
    spdlog::debug("LeastSquaresMonteCarlo constructor called with params: "
                 "spot={}, strike={}, rate={}, vol={}, time={}, paths={}, dates={}", 
                 spot, strike, rate, volatility, time, paths, exerciseDates);
}

double LeastSquaresMonteCarlo::calculatePrice() const {
    spdlog::debug("Calculating option price using Least Squares Monte Carlo method");
    auto paths = generatePaths();
    double price = regressionAndDiscounting(paths);
    spdlog::info("Option price calculated: {}", price);
    return price;
}

std::string LeastSquaresMonteCarlo::getModelName() const {
    return "Least Squares Monte Carlo";
}

std::vector<std::vector<double>> LeastSquaresMonteCarlo::generatePaths() const {
    spdlog::debug("Generating {} paths with {} exercise dates", numPaths, numExerciseDates);
    
    std::vector<std::vector<double>> paths(numPaths, 
                                         std::vector<double>(numExerciseDates));
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> norm(0.0, 1.0);
    
    double deltaT = timeToMaturity / numExerciseDates;
    double drift = (riskFreeRate - 0.5 * volatility * volatility) * deltaT;
    double diffusion = volatility * sqrt(deltaT);
    
    for (int i = 0; i < numPaths; ++i) {
        paths[i][0] = spotPrice;
        for (int j = 1; j < numExerciseDates; ++j) {
            double z = norm(gen);
            paths[i][j] = paths[i][j-1] * exp(drift + diffusion * z);
        }
    }
    
    spdlog::debug("Path generation completed");
    return paths;
}

double LeastSquaresMonteCarlo::regressionAndDiscounting(
    const std::vector<std::vector<double>>& paths) const {
    
    spdlog::debug("Starting regression and discounting for {} paths with {} exercise dates", 
                 numPaths, numExerciseDates);
    
    // Initialize continuation values
    std::vector<double> continuationValues(numPaths, 0.0);
    std::vector<double> exerciseValues(numPaths, 0.0);
    
    // Calculate discount factor
    double deltaT = timeToMaturity / numExerciseDates;
    double discount = exp(-riskFreeRate * deltaT);
    
    // Work backwards through exercise dates
    for (int t = numExerciseDates - 1; t >= 0; --t) {
        spdlog::debug("Processing exercise date {}", t);
        
        // Calculate exercise values at this time step
        for (int i = 0; i < numPaths; ++i) {
            exerciseValues[i] = std::max(strikePrice - paths[i][t], 0.0);
        }
        
        // Identify in-the-money paths
        std::vector<size_t> itm_indices;
        for (int i = 0; i < numPaths; ++i) {
            if (exerciseValues[i] > 0) {
                itm_indices.push_back(i);
            }
        }
        
        if (!itm_indices.empty()) {
            // Perform regression for in-the-money paths
            std::vector<double> X(itm_indices.size());
            std::vector<double> Y(itm_indices.size());
            
            for (size_t i = 0; i < itm_indices.size(); ++i) {
                X[i] = paths[itm_indices[i]][t];
                Y[i] = continuationValues[itm_indices[i]] * discount;
            }
            
            // Calculate regression coefficients
            double sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumX2 = 0.0;
            for (size_t i = 0; i < itm_indices.size(); ++i) {
                sumX += X[i];
                sumY += Y[i];
                sumXY += X[i] * Y[i];
                sumX2 += X[i] * X[i];
            }
            
            double n = static_cast<double>(itm_indices.size());
            double beta1 = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
            double beta0 = (sumY - beta1 * sumX) / n;
            
            // Update continuation values based on regression
            for (size_t i = 0; i < itm_indices.size(); ++i) {
                double expectedContinuation = beta0 + beta1 * X[i];
                if (exerciseValues[itm_indices[i]] > expectedContinuation) {
                    continuationValues[itm_indices[i]] = exerciseValues[itm_indices[i]];
                }
            }
        }
        
        spdlog::debug("Completed regression for exercise date {}", t);
    }
    
    // Calculate final option value
    double optionValue = std::accumulate(continuationValues.begin(), 
                                       continuationValues.end(), 0.0) / numPaths;
    
    spdlog::info("Final option value calculated: {}", optionValue);
    return optionValue;
}
