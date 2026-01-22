#include "contract/coupon/floatingCoupon.hpp"

#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <cmath>

namespace qris::contract {

FloatingCoupon::FloatingCoupon(std::shared_ptr<const qris::core::RateIndex> index,
                               double spread,
                               qris::time::DayCountConvention dayCount)
    : index_(std::move(index)),
      spread_(spread),
      dayCount_(dayCount)
{
    // -- Validation de l'index --
    if (!index_) {
        throw std::invalid_argument(
            "FloatingCoupon: RateIndex pointer must not be null");
    }

    // --- Validation du spread ---
    if (!std::isfinite(spread_)) {
        throw std::invalid_argument(
            "FloatingCoupon: spread must be a finite number");
    }
}

} // namespace qris::contract
