#include "contract/coupon/fixedCoupon.hpp"
#include <cmath>
#include <stdexcept>

namespace qris::contract {

FixedCoupon::FixedCoupon(double fixedRate,
                         qris::time::DayCountConvention dayCount)
    : fixedRate_(fixedRate),
      dayCount_(dayCount)
{
    // --- Validation du taux fixe ---
    if (std::isnan(fixedRate_)) {
        throw std::invalid_argument("FixedCoupon: fixed rate cannot be NaN");
    }

    if (std::isinf(fixedRate_)) {
        throw std::invalid_argument("FixedCoupon: fixed rate cannot be infinite");
    }

    // --- Validation implicite de la DayCountConvention ---
}



} // namespace qris::contract

