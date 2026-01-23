#pragma once 

#include "contract/leg/leg.hpp"
#include "contract/coupon/floatingCoupon.hpp"

namespace qris::contract {

/**
 * @brief Leg à taux flottants (objet contractuel pur). 
 * 
 * - Génère ses cashflows à la construction 
 * - coupon partagé entre tous les flux
 * - Aucun calcul financier
 */

class FloatingLeg final : public Leg {
public: 
    FloatingLeg(
        qris::time::Date startDate, 
        qris::time::Date endDate, 
        qris::time::Period accrualPeriod, 
        qris::time::Calendar calendar, 
        qris::time::BusinessDayConvention bdc, 
        qris::time::StubRule stubRule, 
        int paymentDelay, 
        int fixingLag, 
        double notional, 
        qris::core::Currency currency, 
        qris::core::PayReceive payReceive, 
        std::shared_ptr<const qris::core::RateIndex> index,
        double spread, 
        qris::time::DayCountConvention dayCount
    ); 
private: 
    std::shared_ptr<const FloatingCoupon> coupon_; 
};

} // namespace qris::contract