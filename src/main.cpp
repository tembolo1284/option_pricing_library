// src/main.cpp
// src/main.cpp
#include <iostream>
#include <iomanip>
#include <spdlog/spdlog.h>

// Include all models
#include "models/BinomialTree.hpp"
#include "models/BlackScholes.hpp"
#include "models/MonteCarlo.hpp"
#include "models/LeastSquaresMonteCarlo.hpp"
#include "models/FiniteDifference.hpp"
#include "models/BermudanOption.hpp"

// Include server
#include "server/Server.hpp"

void printOptionPrice(const std::string& modelName, double price) {
    spdlog::info("{:<30} Price: {:.6f}", modelName, price);
}

void runPricingExamples() {
    spdlog::info("Running option pricing examples...\n");

    // Common option parameters
    double spotPrice = 100.0;           // Current stock price
    double strikePrice = 100.0;         // Strike price
    double riskFreeRate = 0.05;         // 5% risk-free rate
    double volatility = 0.2;            // 20% volatility
    double timeToMaturity = 1.0;        // 1 year to expiry

    spdlog::info("Option Parameters:");
    spdlog::info("Spot Price: {}", spotPrice);
    spdlog::info("Strike Price: {}", strikePrice);
    spdlog::info("Risk-free Rate: {:.1f}%", riskFreeRate * 100);
    spdlog::info("Volatility: {:.1f}%", volatility * 100);
    spdlog::info("Time to Maturity: {} year\n", timeToMaturity);

    try {
        // 1. Black-Scholes (European option)
        BlackScholes bs(spotPrice, strikePrice, riskFreeRate, volatility, timeToMaturity);
        printOptionPrice(bs.getModelName(), bs.calculatePrice());

        // 2. Binomial Tree (supports American options)
        int numSteps = 100;
        BinomialTree bt(spotPrice, strikePrice, riskFreeRate, volatility, timeToMaturity, numSteps, true);
        printOptionPrice(bt.getModelName(), bt.calculatePrice());

        // 3. Monte Carlo (path-dependent options)
        int numPaths = 10000;
        bool isAsian = true;  // Price an Asian option
        MonteCarlo mc(spotPrice, strikePrice, riskFreeRate, volatility, timeToMaturity, numPaths, isAsian);
        printOptionPrice(mc.getModelName(), mc.calculatePrice());

        // 4. Least Squares Monte Carlo (American options)
        int numExerciseDates = 50;
        LeastSquaresMonteCarlo lsmc(spotPrice, strikePrice, riskFreeRate, volatility, 
                                   timeToMaturity, numPaths, numExerciseDates);
        printOptionPrice(lsmc.getModelName(), lsmc.calculatePrice());

        // 5. Finite Difference (flexible for various options)
        int gridSize = 100;
        FiniteDifference fd(spotPrice, strikePrice, riskFreeRate, volatility, 
                           timeToMaturity, gridSize, FiniteDifference::Scheme::CrankNicolson);
        printOptionPrice(fd.getModelName(), fd.calculatePrice());

        // 6. Bermudan Option (discrete exercise dates)
        int bermudanExerciseDates = 4;  // Quarterly exercise
        BermudanOption bermudan(spotPrice, strikePrice, riskFreeRate, volatility, 
                               timeToMaturity, bermudanExerciseDates);
        printOptionPrice(bermudan.getModelName(), bermudan.calculatePrice());

    } catch (const std::exception& e) {
        spdlog::error("Error during pricing: {}", e.what());
    }
}

int main() {
    try {
        // Set up logging
        spdlog::set_level(spdlog::level::debug);
        spdlog::info("Starting Option Pricing Server...");

        // Run pricing examples
        runPricingExamples();

        // Start server
        Server server("localhost", 50051);
        server.start();
        spdlog::info("Server is running on localhost:50051");
        spdlog::info("Press Enter to stop the server...");

        // Better way to handle server lifecycle
        std::string input;
        std::getline(std::cin, input);

        server.stop();
        spdlog::info("Server stopped.");
        return 0;

    } catch (const std::exception& e) {
        spdlog::error("Fatal error: {}", e.what());
        return 1;
    }
}
