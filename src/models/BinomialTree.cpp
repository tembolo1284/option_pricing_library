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
    calculateProbUp();
}

void BinomialTree::calculateProbUp() {
    double deltaT = timeToMaturity / numSteps;
    double upFactor = std::exp(volatility * std::sqrt(deltaT));
    double downFactor = 1.0 / upFactor;
    
    // Calculate risk-neutral probability
    probUp = (std::exp(riskFreeRate * deltaT) - downFactor) / (upFactor - downFactor);
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
    std::vector<std::vector<double>> tree(numSteps + 1);
    
    double deltaT = timeToMaturity / numSteps;
    double upFactor = std::exp(volatility * std::sqrt(deltaT));
    double downFactor = 1.0 / upFactor;
    
    spdlog::debug("Tree parameters: upFactor={}, downFactor={}, probUp={}",
                 upFactor, downFactor, probUp);
    
    // Build the stock price tree
    for (int i = 0; i <= numSteps; ++i) {
        tree[i].resize(i + 1);
        for (int j = 0; j <= i; ++j) {
            int up_moves = j;
            int down_moves = i - j;
            tree[i][j] = spotPrice * std::pow(upFactor, up_moves) * std::pow(downFactor, down_moves);
        }
    }
    
    spdlog::debug("Binomial tree construction completed");
    return tree;
}

double BinomialTree::backwardInduction(const std::vector<std::vector<double>>& tree) const {
    spdlog::debug("Starting backward induction");
    std::vector<double> optionValues(numSteps + 1);
    
    // Initialize terminal values (using max of call option payoff)
    for (int j = 0; j <= numSteps; ++j) {
        // Call option payoff
        optionValues[j] = std::max(0.0, tree[numSteps][j] - strikePrice);
    }
    
    double deltaT = timeToMaturity / numSteps;
    double discountFactor = std::exp(-riskFreeRate * deltaT);
    
    spdlog::debug("Backward induction parameters: discountFactor={}, probUp={}",
                 discountFactor, probUp);
    
    // Backward induction
    for (int i = numSteps - 1; i >= 0; --i) {
        for (int j = 0; j <= i; ++j) {
            // Calculate continuation value
            double continuationValue = discountFactor * 
                (probUp * optionValues[j + 1] + (1 - probUp) * optionValues[j]);
            
            if (americanExercise) {
                // For American options, compare with immediate exercise value
                double exerciseValue = std::max(0.0, tree[i][j] - strikePrice);
                optionValues[j] = std::max(continuationValue, exerciseValue);
            } else {
                optionValues[j] = continuationValue;
            }
        }
    }
    
    spdlog::debug("Backward induction completed");
    return optionValues[0];
}
