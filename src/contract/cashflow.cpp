#include "contract/cashflow.hpp"
#include "contract/coupon/coupon.hpp"

#include <stdexcept>
#include <utility>

namespace qris::contract {

Cashflow::Cashflow(
    Date paymentDate,
    std::optional<Date> fixingDate,
    std::optional<Date> accrualStartDate,
    std::optional<Date> accrualEndDate,
    qris::core::Currency currency,
    double notional,
    qris::core::PayReceive direction,
    std::unique_ptr<const Coupon> coupon)
    : paymentDate_(std::move(paymentDate))
    , fixingDate_(std::move(fixingDate))
    , accrualStartDate_(std::move(accrualStartDate))
    , accrualEndDate_(std::move(accrualEndDate))
    , currency_(std::move(currency))
    , notional_(notional)
    , direction_(direction)
    , coupon_(std::move(coupon))
{
    validateInvariants();
}

void Cashflow::validateInvariants() const
{
    // Coupon obligatoire
    if (!coupon_) {
        throw std::invalid_argument(
            "Cashflow: coupon must not be null");
    }

    // Notional strictement positif
    if (notional_ <= 0.0) {
        throw std::invalid_argument(
            "Cashflow: notional must be strictly positive");
    }

    // Cohérence de la période d'accrual
    if (accrualStartDate_ && accrualEndDate_) {
        if (*accrualEndDate_ <= *accrualStartDate_) {
            throw std::invalid_argument(
                "Cashflow: accrualEndDate must be strictly after accrualStartDate");
        }
    }

    // Fixing antérieur ou égal au début d'accrual
    if (fixingDate_ && accrualStartDate_) {
        if (*fixingDate_ > *accrualStartDate_) {
            throw std::invalid_argument(
                "Cashflow: fixingDate must be on or before accrualStartDate");
        }
    }

    // Paiement après la fin de la période d'accrual
    if (accrualEndDate_) {
        if (paymentDate_ < *accrualEndDate_) {
            throw std::invalid_argument(
                "Cashflow: paymentDate must be on or after accrualEndDate");
        }
    }
}

} // namespace qris::contract
