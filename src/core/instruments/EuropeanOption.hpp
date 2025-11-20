#pragma once

#include <stdexcept>

namespace qris {
namespace instruments {

    enum class OptionType {
        Call,
        Put
    };

    /// @brief Représentation d'une option européenne vanille.
    class EuropeanOption {
    public:
        EuropeanOption(double strike,
                       double maturity,
                       OptionType type)
            : strike_(strike),
              maturity_(maturity),
              type_(type)
        {
            if (strike_ <= 0.0)
                throw std::invalid_argument("Strike must be strictly positive.");

            if (maturity_ <= 0.0)
                throw std::invalid_argument("Maturity must be strictly positive.");
        }

        // --- Getters (pas de setters : instrument immuable) ---
        double get_strike() const noexcept { return strike_; }
        double get_maturity() const noexcept { return maturity_; }
        OptionType get_type() const noexcept { return type_; }

        // Compatibility aliases: short names used by some tests/examples
        double strike() const noexcept { return get_strike(); }
        double maturity() const noexcept { return get_maturity(); }
        OptionType type() const noexcept { return get_type(); }

        // Helpers utiles pour le pricer
        bool isCall() const noexcept { return type_ == OptionType::Call; }
        bool isPut()  const noexcept { return type_ == OptionType::Put; }

    private:
        double strike_;
        double maturity_;
        OptionType type_;
    };

} // namespace instruments
} // namespace qris
