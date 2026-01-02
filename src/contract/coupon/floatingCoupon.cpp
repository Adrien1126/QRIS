#include "contract/coupon/floatingCoupon.hpp"

#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <cmath>

namespace qris::contract {

FloatingCoupon::FloatingCoupon(std::string indexName,
                               double spread,
                               qris::time::DayCountConvention dayCount)
    : indexName_(std::move(indexName)),
      spread_(spread),
      dayCount_(dayCount)
{
    // --- Validation de l'index ---
    if (indexName_.empty()) {
        throw std::invalid_argument(
            "FloatingCoupon: index name must not be empty");
    }

    // Vérifie que l'index n'est pas composé uniquement d'espaces
    if (std::all_of(indexName_.begin(), indexName_.end(),
                    [](unsigned char c) { return std::isspace(c); })) {
        throw std::invalid_argument(
            "FloatingCoupon: index name must not be blank");
    }

    // --- Validation du spread ---
    if (!std::isfinite(spread_)) {
        throw std::invalid_argument(
            "FloatingCoupon: spread must be a finite number");
    }
}

} // namespace qris::contract
