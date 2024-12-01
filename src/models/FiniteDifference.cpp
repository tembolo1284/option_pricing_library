// src/models/FiniteDifference.cpp
#include "models/FiniteDifference.hpp"
#include <cmath>
#include <stdexcept>
#include <spdlog/spdlog.h>
#include <vector>
#include <algorithm>

FiniteDifference::FiniteDifference(double spot, double strike, double rate, 
                                 double volatility, double time, int gridSize, 
                                 Scheme scheme)
    : spotPrice(spot), strikePrice(strike), riskFreeRate(rate), 
      volatility(volatility), timeToMaturity(time), gridSize(gridSize),
      fdScheme(scheme) {
    spdlog::debug("FiniteDifference constructor called with params: spot={}, strike={}, "
                 "rate={}, vol={}, time={}, gridSize={}", 
                 spot, strike, rate, volatility, time, gridSize);
}

double FiniteDifference::calculatePrice() const {
    spdlog::debug("Calculating option price using Finite Difference method");
    auto grid = buildGrid();
    solveGrid(grid);
    // Price at spot price level (middle of the grid)
    double price = grid[0][gridSize/2];  
    spdlog::info("Option price calculated: {}", price);
    return price;
}

std::string FiniteDifference::getModelName() const {
    return "Finite Difference";
}

std::vector<std::vector<double>> FiniteDifference::buildGrid() const {
    spdlog::debug("Building finite difference grid with size {}", gridSize);
    
    std::vector<std::vector<double>> grid(gridSize + 1, 
                                        std::vector<double>(gridSize + 1, 0.0));
    
    double deltaS = 2.0 * spotPrice / gridSize;
    
    // Set terminal conditions (option payoff at maturity)
    for (int i = 0; i <= gridSize; ++i) {
        double S = i * deltaS;
        grid[gridSize][i] = std::max(strikePrice - S, 0.0);  // Put option payoff
    }
    
    spdlog::debug("Grid initialization completed");
    return grid;
}

// Helper function to solve tridiagonal system using Thomas algorithm
std::vector<double> solveTridiagonal(
    const std::vector<double>& a,  // lower diagonal
    const std::vector<double>& b,  // main diagonal
    const std::vector<double>& c,  // upper diagonal
    std::vector<double> d          // right hand side
) {
    int n = d.size();
    std::vector<double> c_prime(n, 0.0);
    std::vector<double> d_prime(n, 0.0);
    
    // Forward sweep
    c_prime[0] = c[0] / b[0];
    d_prime[0] = d[0] / b[0];
    
    for(int i = 1; i < n; i++) {
        double m = 1.0 / (b[i] - a[i] * c_prime[i-1]);
        c_prime[i] = c[i] * m;
        d_prime[i] = (d[i] - a[i] * d_prime[i-1]) * m;
    }
    
    // Back substitution
    std::vector<double> x(n, 0.0);
    x[n-1] = d_prime[n-1];
    
    for(int i = n-2; i >= 0; i--) {
        x[i] = d_prime[i] - c_prime[i] * x[i+1];
    }
    
    return x;
}

void FiniteDifference::solveGrid(std::vector<std::vector<double>>& grid) const {
    spdlog::debug("Starting grid solution using {} scheme", 
                 fdScheme == Scheme::Explicit ? "Explicit" : 
                 fdScheme == Scheme::Implicit ? "Implicit" : "Crank-Nicolson");
    
    double deltaT = timeToMaturity / gridSize;
    double deltaS = 2.0 * spotPrice / gridSize;
    
    // Calculate coefficients for the finite difference scheme
    double alpha = 0.5 * riskFreeRate * deltaT / deltaS;
    double beta = 0.5 * volatility * volatility * deltaT / (deltaS * deltaS);
    
    if (fdScheme == Scheme::Explicit) {
        spdlog::debug("Using explicit finite difference method");
        
        // Check stability condition for explicit scheme
        if (beta > 0.5) {
            spdlog::error("Stability condition violated: beta = {}", beta);
            throw std::runtime_error("Explicit scheme stability condition violated");
        }
        
        // Implement explicit scheme
        for (size_t t = grid.size() - 2; t > 0; --t) {
            for (size_t i = 1; i < grid[t].size() - 1; ++i) {
                double Si = i * deltaS;
                grid[t][i] = grid[t + 1][i] + 
                            alpha * Si * (grid[t + 1][i + 1] - grid[t + 1][i - 1]) +
                            beta * (grid[t + 1][i + 1] - 2.0 * grid[t + 1][i] + grid[t + 1][i - 1]);
            }
        }
    } else if (fdScheme == Scheme::Implicit) {
        spdlog::debug("Using implicit finite difference method");
        
        int n = gridSize - 1; // Number of internal points
        std::vector<double> a(n, 0.0); // lower diagonal
        std::vector<double> b(n, 0.0); // main diagonal
        std::vector<double> c(n, 0.0); // upper diagonal
        std::vector<double> d(n, 0.0); // right hand side
        
        // Set up coefficients for implicit scheme
        for (int i = 0; i < n; i++) {
            double Si = (i + 1) * deltaS;
            double ai = beta - alpha * Si / 2.0;
            double bi = -1.0 - 2.0 * beta;
            double ci = beta + alpha * Si / 2.0;
            
            a[i] = ai;
            b[i] = bi;
            c[i] = ci;
        }
        
        // Time stepping
        for (size_t t = grid.size() - 2; t > 0; --t) {
            // Set up right hand side
            for (int i = 0; i < n; i++) {
                d[i] = -grid[t + 1][i + 1];
            }
            
            // Solve tridiagonal system
            std::vector<double> solution = solveTridiagonal(a, b, c, d);
            
            // Update grid
            for (int i = 0; i < n; i++) {
                grid[t][i + 1] = solution[i];
            }
        }
    } else { // Crank-Nicolson
        spdlog::debug("Using Crank-Nicolson scheme");
        
        int n = gridSize - 1;
        std::vector<double> a(n, 0.0);
        std::vector<double> b(n, 0.0);
        std::vector<double> c(n, 0.0);
        std::vector<double> d(n, 0.0);
        
        // Set up coefficients for Crank-Nicolson scheme
        for (int i = 0; i < n; i++) {
            double Si = (i + 1) * deltaS;
            double ai = 0.5 * (beta - alpha * Si / 2.0);
            double bi = -1.0 - beta;
            double ci = 0.5 * (beta + alpha * Si / 2.0);
            
            a[i] = ai;
            b[i] = bi;
            c[i] = ci;
        }
        
        // Time stepping
        for (size_t t = grid.size() - 2; t > 0; --t) {
            // Set up right hand side using previous time step
            for (int i = 0; i < n; i++) {
                double Si = (i + 1) * deltaS;
                double prev_val = grid[t + 1][i + 1];
                double prev_up = grid[t + 1][i + 2];
                double prev_down = grid[t + 1][i];
                
                d[i] = -(prev_val + 0.5 * (
                    alpha * Si * (prev_up - prev_down) +
                    beta * (prev_up - 2.0 * prev_val + prev_down)
                ));
            }
            
            // Solve tridiagonal system
            std::vector<double> solution = solveTridiagonal(a, b, c, d);
            
            // Update grid
            for (int i = 0; i < n; i++) {
                grid[t][i + 1] = solution[i];
            }
        }
    }
    
    spdlog::debug("Grid solution completed");
}
