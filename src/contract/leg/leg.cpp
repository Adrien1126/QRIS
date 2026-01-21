#include "contract/leg/leg.hpp"

#include <stdexcept>

namespace qris::contract {

// -----------------------------------------------------------------------------
// Constructeur
// -----------------------------------------------------------------------------

Leg::Leg(
    qris::time::Date startDate,
    qris::time::Date endDate,
    qris::time::Period accrualPeriod,
    qris::time::Calendar calendar,
    qris::time::BusinessDayConvention bdc,
    qris::time::StubRule stubRule,
    int paymentDelay,
    double notional,
    qris::core::Currency currency,
    qris::core::PayReceive payReceive
)
: startDate_(startDate)
, endDate_(endDate)
, accrualPeriod_(accrualPeriod)
, calendar_(calendar)
, bdc_(bdc)
, stubRule_(stubRule)
, paymentDelay_(paymentDelay)
, notional_(notional)
, currency_(currency)
, payReceive_(payReceive)
{
    // Invariants contractuels minimaux

    if (!(startDate_ < endDate_)) {
        throw std::invalid_argument(
            "Leg: startDate must be strictly before endDate");
    }

    if (paymentDelay_ < 0) {
        throw std::invalid_argument(
            "Leg: paymentDelay must be non-negative");
    }

    if (notional_ <= 0.0) {
        throw std::invalid_argument(
            "Leg: notional must be strictly positive");
    }
}

// -----------------------------------------------------------------------------
// Validation des invariants globaux
// -----------------------------------------------------------------------------

void Leg::validateInvariants() const
{
    if (cashflows_.empty()) {
        throw std::logic_error(
            "Leg: cashflows must not be empty");
    }

    // Payment dates must be non-decreasing (NOT strictly increasing)
    for (std::size_t i = 1; i < cashflows_.size(); ++i) {
        if (cashflows_[i].paymentDate() <
            cashflows_[i - 1].paymentDate()) {
            throw std::logic_error(
                "Leg: payment dates must be non-decreasing");
        }
    }
}

} // namespace qris::contract
