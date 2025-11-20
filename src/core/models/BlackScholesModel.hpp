#pragma once

#include <stdexcept>
#include <cmath>

namespace qris {
namespace model {

    /// @brief Modèle Black–Scholes de base :
    /// stocke spot, taux, volatilité
    /// et calcule d1 et d2.
    class BlackScholesModel {
    public:

        BlackScholesModel(double spot,
                          double rate,
                          double volatility)
            : spot_(spot),
              rate_(rate),
              vol_(volatility)
        {
            if (spot_ <= 0.0)
                throw std::invalid_argument("Spot must be strictly positive.");

            if (vol_ <= 0.0)
                throw std::invalid_argument("Volatility must be strictly positive.");
        }

        // --- Getters ---
        double get_spot() const noexcept { return spot_; }
        double get_rate() const noexcept { return rate_; }
        double get_volatility() const noexcept { return vol_; }

        // --- d1 / d2 ---
        double d1(double strike, double maturity) const;
        double d2(double strike, double maturity) const;

    private:
        double spot_;
        double rate_;
        double vol_;
    };

} // namespace model
} // namespace qris
