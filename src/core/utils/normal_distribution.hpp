#pragma once

#include <cmath>

namespace qris {
namespace math {

/// Helpers for the standard normal distribution N(0,1).
class NormalDistribution {
public:
    /// densité φ(x)
    static double pdf(double x) noexcept;

    /// répartition Φ(x)
    static double cdf(double x) noexcept;
};

} // namespace math
} // namespace qris
