#pragma once

#include "contract/leg/leg.hpp"
#include "contract/coupon/fixedCoupon.hpp"

namespace qris::contract {

/**
 * @brief Leg à taux fixe (objet contractuel pur).
 *
 * - Génère ses cashflows à la construction
 * - Coupon partagé entre tous les flux
 * - Aucun calcul financier
 */
class FixedLeg final : public Leg {
public:
    FixedLeg(
        qris::time::Date startDate,
        qris::time::Date endDate,
        qris::time::Period accrualPeriod,
        qris::time::Calendar calendar,
        qris::time::BusinessDayConvention bdc,
        qris::time::StubRule stubRule,
        int paymentDelay,
        double notional,
        qris::core::Currency currency,
        qris::core::PayReceive payReceive,
        double fixedRate,
        qris::time::DayCountConvention dayCount
    );

private:
    std::shared_ptr<const FixedCoupon> coupon_;
};

} // namespace qris::contract
