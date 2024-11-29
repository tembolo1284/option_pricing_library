// include/models/BermudanOption.hpp
#ifndef BERMUDAN_OPTION_HPP
#define BERMUDAN_OPTION_HPP

#include <string>

class BermudanOption {
public:
    BermudanOption(double spot, double strike, double rate, double volatility, double time, int numExerciseDates);
    double calculatePrice() const;
    std::string getModelName() const;

private:
    double spotPrice;
    double strikePrice;
    double riskFreeRate;
    double volatility;
    double timeToMaturity;
    int numExerciseDates;

    double backwardInduction() const;
};

#endif // BERMUDAN_OPTION_HPP

