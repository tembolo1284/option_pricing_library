// include/models/BlackScholes.hpp
#ifndef BLACK_SCHOLES_HPP
#define BLACK_SCHOLES_HPP

#include <string>

class BlackScholes {
public:
    BlackScholes(double spot, double strike, double rate, double volatility, double time);
    double calculatePrice() const;
    std::string getModelName() const;

private:
    double spotPrice;
    double strikePrice;
    double riskFreeRate;
    double volatility;
    double timeToMaturity;

    double calculateD1() const;
    double calculateD2() const;
    double normalCDF(double value) const;
};

#endif // BLACK_SCHOLES_HPP

