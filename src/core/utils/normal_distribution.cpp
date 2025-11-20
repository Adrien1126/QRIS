// src/core/utils/normal_distribution.cpp
#include "normal_distribution.hpp"

namespace qris {
namespace math {

    namespace {
        // Constante 1 / sqrt(2π)
        constexpr double INV_SQRT_2PI = 0.39894228040143267793994605993438;
        constexpr double INV_SQRT_2   = 0.70710678118654752440084436210485;
    }

    double NormalDistribution::pdf(double x) noexcept {
        // φ(x) = 1/sqrt(2π) * exp(-x²/2)
        return INV_SQRT_2PI * std::exp(-0.5 * x * x);
    }

    double NormalDistribution::cdf(double x) noexcept {
        // Φ(x) = 0.5 * erfc(-x / sqrt(2))
        // erfc(y) = 1 - erf(y)
        return 0.5 * std::erfc(-x * INV_SQRT_2);
    }

} // namespace math
} // namespace qris
