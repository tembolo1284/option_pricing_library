// include/models/BinomialTree.hpp
#ifndef BINOMIAL_TREE_HPP
#define BINOMIAL_TREE_HPP

#include <vector>
#include <string>
#include <spdlog/spdlog.h>

class BinomialTree {
public:
    BinomialTree(double spot, double strike, double rate, double volatility, double time, int steps, bool isAmerican);
    double calculatePrice() const;
    std::string getModelName() const;

private:
    double spotPrice;
    double strikePrice;
    double riskFreeRate;
    double volatility;
    double timeToMaturity;
    int numSteps;
    bool americanExercise;
    double probUp;

    std::vector<std::vector<double>> buildTree() const;
    double backwardInduction(const std::vector<std::vector<double>>& tree) const;
    void calculateProbUp();
};

#endif // BINOMIAL_TREE_HPP

