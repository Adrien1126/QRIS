#pragma once

#include <string>
#include "contract/coupon/coupon.hpp"

namespace qris::contract {

/**
 * @brief Coupon indexé sur un taux de référence.
 */
class FloatingCoupon final : public Coupon {
private:
    std::string indexName_;   // ex: "EURIBOR_3M"
    double spread_;
    qris::time::DayCountConvention dayCount_;

public:
    FloatingCoupon(std::string indexName,
                   double spread,
                   qris::time::DayCountConvention dayCount);

    CouponType type() const override;
    qris::time::DayCountConvention dayCount() const override;

    const std::string& index() const;
    double spread() const;
};

}
