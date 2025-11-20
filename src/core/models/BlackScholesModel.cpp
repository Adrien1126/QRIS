#include "BlackScholesModel.hpp"

namespace qris {
namespace model {

    double BlackScholesModel::d1(double strike, double maturity) const {
        if (strike <= 0.0)
            throw std::invalid_argument("Strike must be strictly positive.");

        if (maturity <= 0.0)
            throw std::invalid_argument("Maturity must be strictly positive.");

        const double num =
            std::log(spot_ / strike) +
            (rate_ + 0.5 * vol_ * vol_) * maturity;

        const double denom = vol_ * std::sqrt(maturity);

        return num / denom;
    }

    double BlackScholesModel::d2(double strike, double maturity) const {
        return d1(strike, maturity) - vol_ * std::sqrt(maturity);
    }

} // namespace model
} // namespace qris
