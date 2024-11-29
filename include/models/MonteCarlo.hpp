// include/models/MonteCarlo.hpp
#ifndef MONTE_CARLO_HPP
#define MONTE_CARLO_HPP

#include <string>
#include <vector>

class MonteCarlo {
public:
    MonteCarlo(double spot, double strike, double rate, double volatility, double time, int numPaths, bool isAsian);
    double calculatePrice() const;
    std::string getModelName() const;

private:
    double spotPrice;
    double strikePrice;
    double riskFreeRate;
    double volatility;
    double timeToMaturity;
    int numPaths;
    bool asianOption;

    double calculateAsianPrice() const;
    std::vector<double> generatePricePath() const;
};

#endif // MONTE_CARLO_HPP

