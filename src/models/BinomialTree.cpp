// src/models/BinomialTree.cpp
#include "models/BinomialTree.hpp"
#include <cmath>
#include <stdexcept>
#include <spdlog/spdlog.h>

BinomialTree::BinomialTree(double spot, double strike, double rate, double volatility, 
                          double time, int steps, bool isAmerican)
    : spotPrice(spot), strikePrice(strike), riskFreeRate(rate), volatility(volatility), 
      timeToMaturity(time), numSteps(steps), americanExercise(isAmerican) 
{
    spdlog::debug("BinomialTree constructor called with parameters: spot={}, strike={}, "
                 "rate={}, vol={}, time={}, steps={}, isAmerican={}", 
                 spot, strike, rate, volatility, time, steps, isAmerican);
    calculateProbUp();  // Initialize probUp in constructor
}

void BinomialTree::calculateProbUp() {
    double deltaT = timeToMaturity / numSteps;
    double upFactor = exp(volatility * sqrt(deltaT));
    double downFactor = 1.0 / upFactor;
    probUp = (exp(riskFreeRate * deltaT) - downFactor) / (upFactor - downFactor);
    spdlog::debug("Calculated probUp: {}", probUp);
}

double BinomialTree::calculatePrice() const {
    spdlog::debug("Calculating option price using Binomial Tree model");
    auto tree = buildTree();
    auto price = backwardInduction(tree);
    spdlog::info("Option price calculated: {}", price);
    return price;
}

std::string BinomialTree::getModelName() const {
    return "Binomial Tree";
}

std::vector<std::vector<double>> BinomialTree::buildTree() const {
    spdlog::debug("Building binomial tree with {} steps", numSteps);
    
    std::vector<std::vector<double>> tree(numSteps + 1, std::vector<double>(numSteps + 1));
    
    double deltaT = timeToMaturity / numSteps;
    double upFactor = exp(volatility * sqrt(deltaT));
    double downFactor = 1.0 / upFactor;
    
    spdlog::debug("Tree parameters: upFactor={}, downFactor={}, probUp={}", 
                 upFactor, downFactor, probUp);

    for (int i = 0; i <= numSteps; ++i) {
        for (int j = 0; j <= i; ++j) {
            tree[i][j] = spotPrice * pow(upFactor, j) * pow(downFactor, i - j);
        }
    }

    spdlog::debug("Binomial tree construction completed");
    return tree;
}

double BinomialTree::backwardInduction(const std::vector<std::vector<double>>& tree) const {
    spdlog::debug("Starting backward induction");
    
    std::vector<double> optionValues(numSteps + 1);
    
    // Initialize terminal values
    for (int j = 0; j <= numSteps; ++j) {
        optionValues[j] = std::max(0.0, americanExercise ? 
            (tree[numSteps][j] - strikePrice) : (tree[numSteps][j] - strikePrice));
    }

    double deltaT = timeToMaturity / numSteps;
    double discountFactor = exp(-riskFreeRate * deltaT);
    
    spdlog::debug("Backward induction parameters: discountFactor={}, probUp={}", 
                 discountFactor, probUp);

    // Backward induction
    for (int i = numSteps - 1; i >= 0; --i) {
        for (int j = 0; j <= i; ++j) {
            double continuationValue = discountFactor * 
                (optionValues[j] * probUp + optionValues[j + 1] * (1 - probUp));
            
            if (americanExercise) {
                double exerciseValue = tree[i][j] - strikePrice;
                optionValues[j] = std::max(continuationValue, exerciseValue);
            } else {
                optionValues[j] = continuationValue;
            }
        }
    }

    spdlog::debug("Backward induction completed");
    return optionValues[0];
}
