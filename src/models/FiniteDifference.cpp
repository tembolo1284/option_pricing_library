// src/models/FiniteDifference.cpp
#include "models/FiniteDifference.hpp"
#include <vector>
#include <cmath>
#include <string>

FiniteDifference::FiniteDifference(double spot, double strike, double rate, double volatility, double time, int gridSize, Scheme scheme)
    : spotPrice(spot), strikePrice(strike), riskFreeRate(rate), volatility(volatility), timeToMaturity(time),
      gridSize(gridSize), fdScheme(scheme) {}

double FiniteDifference::calculatePrice() const {
    auto grid = buildGrid();
    solveGrid(grid);
    return grid[0][0]; // Final price at the root of the grid
}

std::string FiniteDifference::getModelName() const {
    return std::string("Finite Difference - ") + 
           (fdScheme == Scheme::Explicit ? "Explicit" :
            fdScheme == Scheme::Implicit ? "Implicit" : "Crank-Nicolson");
}

std::vector<std::vector<double>> FiniteDifference::buildGrid() const {
    std::vector<std::vector<double>> grid(gridSize, std::vector<double>(gridSize, 0.0));
    return grid; // Placeholder: initialize grid with zeros
}

void FiniteDifference::solveGrid(std::vector<std::vector<double>>& grid) const {
    // Placeholder: Implement Explicit/Implicit/Crank-Nicolson schemes
}

