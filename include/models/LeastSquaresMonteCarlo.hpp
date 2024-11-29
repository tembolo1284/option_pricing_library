// include/models/LeastSquaresMonteCarlo.hpp
#ifndef LEAST_SQUARES_MONTE_CARLO_HPP
#define LEAST_SQUARES_MONTE_CARLO_HPP

#include <string>
#include <vector>

class LeastSquaresMonteCarlo {
public:
    LeastSquaresMonteCarlo(double spot, double strike, double rate, double volatility, double time, int numPaths, int numExerciseDates);
    double calculatePrice() const;
    std::string getModelName() const;

private:
    double spotPrice;
    double strikePrice;
    double riskFreeRate;
    double volatility;
    double timeToMaturity;
    int numPaths;
    int numExerciseDates;

    std::vector<std::vector<double>> generatePaths() const;
    double regressionAndDiscounting(const std::vector<std::vector<double>>& paths) const;
};

#endif // LEAST_SQUARES_MONTE_CARLO_HPP

