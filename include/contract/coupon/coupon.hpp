#pragma once

#include "time/dayCountConvention.hpp"
#include "contract/coupon/couponType.hpp"

namespace qris::contract {

/**
 * @brief Description contractuelle d’un coupon.
 *
 * Un Coupon décrit les règles contractuelles permettant
 * de calculer un montant futur, sans effectuer aucun calcul.
 */
class Coupon {
public:
    virtual ~Coupon() = default;

    /// Type contractuel du coupon
    virtual CouponType type() const = 0;

    /// Convention de day count associée au coupon
    virtual qris::time::DayCountConvention dayCount() const = 0;
};

}
