// include/models/FiniteDifference.hpp
#ifndef FINITE_DIFFERENCE_HPP
#define FINITE_DIFFERENCE_HPP

#include <string>
#include <vector>

class FiniteDifference {
public:
    enum class Scheme { Explicit, Implicit, CrankNicolson };

    FiniteDifference(double spot, double strike, double rate, double volatility, double time, int gridSize, Scheme scheme);
    double calculatePrice() const;
    std::string getModelName() const;

private:
    double spotPrice;
    double strikePrice;
    double riskFreeRate;
    double volatility;
    double timeToMaturity;
    int gridSize;
    Scheme fdScheme;

    std::vector<std::vector<double>> buildGrid() const;
    void solveGrid(std::vector<std::vector<double>>& grid) const;
};

#endif // FINITE_DIFFERENCE_HPP

