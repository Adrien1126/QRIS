#pragma once

#include <string>
#include "contract/coupon/coupon.hpp"
#include "core/types/rateIndex.hpp"

namespace qris::contract {

/**
 * @brief Coupon indexé sur un taux de référence.
 */
class FloatingCoupon final : public Coupon {
private:
    const std::shared_ptr<const qris::core::RateIndex> index_;  // ex: "EURIBOR_3M"
    const double spread_;
    const qris::time::DayCountConvention dayCount_;

public:
    FloatingCoupon(std::shared_ptr<const qris::core::RateIndex> index_,
                   double spread,
                   qris::time::DayCountConvention dayCount);

    CouponType type() const override {return CouponType::Floating;}
    qris::time::DayCountConvention dayCount() const override {return dayCount_;}

    const qris::core::RateIndex& index() const {return *index_;}
    const std::shared_ptr<const qris::core::RateIndex>& indexPtr() const { return index_; }
    
    double spread() const {return spread_;}
};

}
