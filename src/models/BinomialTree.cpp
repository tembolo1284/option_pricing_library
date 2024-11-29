// src/models/BinomialTree.cpp
#include "models/BinomialTree.hpp"
#include <cmath>
#include <stdexcept>

BinomialTree::BinomialTree(double spot, double strike, double rate, double volatility, double time, int steps, bool isAmerican)
    : spotPrice(spot), strikePrice(strike), riskFreeRate(rate), volatility(volatility), timeToMaturity(time),
      numSteps(steps), americanExercise(isAmerican) {}

double BinomialTree::calculatePrice() const {
    auto tree = buildTree();
    return backwardInduction(tree);
}

std::string BinomialTree::getModelName() const {
    return "Binomial Tree";
}

std::vector<std::vector<double>> BinomialTree::buildTree() const {
    std::vector<std::vector<double>> tree(numSteps + 1, std::vector<double>(numSteps + 1));
    double upFactor = exp(volatility * sqrt(timeToMaturity / numSteps));
    double downFactor = 1.0 / upFactor;
    double probUp = (exp(riskFreeRate * timeToMaturity / numSteps) - downFactor) / (upFactor - downFactor);

    for (int i = 0; i <= numSteps; ++i) {
        for (int j = 0; j <= i; ++j) {
            tree[i][j] = spotPrice * pow(upFactor, j) * pow(downFactor, i - j);
        }
    }
    return tree;
}

double BinomialTree::backwardInduction(const std::vector<std::vector<double>>& tree) const {
    std::vector<double> optionValues(numSteps + 1);
    for (int j = 0; j <= numSteps; ++j) {
        optionValues[j] = std::max(0.0, (americanExercise ? spotPrice - strikePrice : 0.0));
    }

    for (int i = numSteps - 1; i >= 0; --i) {
        for (int j = 0; j <= i; ++j) {
            optionValues[j] = exp(-riskFreeRate * timeToMaturity / numSteps) *
                              (optionValues[j] * probUp + optionValues[j + 1] * (1 - probUp));
        }
    }
    return optionValues[0];
}

