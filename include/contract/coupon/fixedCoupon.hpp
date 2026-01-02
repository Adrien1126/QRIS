#pragma once

#include "contract/coupon/coupon.hpp"

namespace qris::contract {

/**
 * @brief Coupon à taux fixe (objet contractuel immuable).
 */
class FixedCoupon final : public Coupon {
private:
    const double fixedRate_;
    const qris::time::DayCountConvention dayCount_;

public:
    FixedCoupon(double fixedRate,
                qris::time::DayCountConvention dayCount);

    CouponType type() const override {
        return CouponType::Fixed;
    }

    qris::time::DayCountConvention dayCount() const override {
        return dayCount_;
    }

    double fixedRate() const noexcept {
        return fixedRate_;
    }
};

} // namespace qris::contract
