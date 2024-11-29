// include/PricingEngine.hpp
#ifndef PRICING_ENGINE_HPP
#define PRICING_ENGINE_HPP

#include <memory>
#include <string>

class PricingEngine {
public:
    PricingEngine();
    double calculatePrice(const std::string& model, const std::vector<double>& parameters);
    std::string getAvailableModels() const;

private:
    std::map<std::string, std::shared_ptr<class PricingModel>> models;

    void registerModel(const std::string& name, std::shared_ptr<class PricingModel> model);
};

#endif // PRICING_ENGINE_HPP

