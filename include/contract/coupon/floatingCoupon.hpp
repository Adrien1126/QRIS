#pragma once

#include <string>
#include "contract/coupon/coupon.hpp"

namespace qris::contract {

/**
 * @brief Coupon indexé sur un taux de référence.
 */
class FloatingCoupon final : public Coupon {
private:
    const std::string indexName_;   // ex: "EURIBOR_3M"
    const double spread_;
    const qris::time::DayCountConvention dayCount_;

public:
    FloatingCoupon(std::string indexName,
                   double spread,
                   qris::time::DayCountConvention dayCount);

    CouponType type() const override {return CouponType::Floating;}
    qris::time::DayCountConvention dayCount() const override {return dayCount_;}

    const std::string& index() const {return indexName_;}
    double spread() const {return spread_;}
};

}
