#include <gtest/gtest.h>

#include "contract/coupon/fixedCoupon.hpp"
#include "time/dayCountConvention.hpp"
#include "contract/coupon/couponType.hpp"

using namespace qris::contract;
using qris::time::DayCountConvention;

// -----------------------------------------------------------------------------
// Construction
// -----------------------------------------------------------------------------

TEST(FixedCouponTest, ConstructValidFixedCoupon)
{
    FixedCoupon coupon(0.025, DayCountConvention::ACT_365F);

    EXPECT_DOUBLE_EQ(coupon.fixedRate(), 0.025);
    EXPECT_EQ(coupon.dayCount(), DayCountConvention::ACT_365F);
    EXPECT_EQ(coupon.type(), CouponType::Fixed);
}

// -----------------------------------------------------------------------------
// Invariants métier
// -----------------------------------------------------------------------------

TEST(FixedCouponTest, FixedRateIsImmutable)
{
    FixedCoupon coupon(0.03, DayCountConvention::ACT_360);

    // On vérifie simplement que le taux reste stable
    EXPECT_DOUBLE_EQ(coupon.fixedRate(), 0.03);
}

TEST(FixedCouponTest, DayCountConventionIsExposedCorrectly)
{
    FixedCoupon coupon(0.015, DayCountConvention::THIRTY_E_360);

    EXPECT_EQ(coupon.dayCount(), DayCountConvention::THIRTY_E_360);
}

// -----------------------------------------------------------------------------
// Polymorphisme via Coupon
// -----------------------------------------------------------------------------

TEST(FixedCouponTest, PolymorphicAccessThroughCouponInterface)
{
    std::unique_ptr<Coupon> coupon =
        std::make_unique<FixedCoupon>(0.02, DayCountConvention::ACT_365F);

    EXPECT_EQ(coupon->type(), CouponType::Fixed);
    EXPECT_EQ(coupon->dayCount(), DayCountConvention::ACT_365F);
}

// -----------------------------------------------------------------------------
// Robustesse conceptuelle (pas de calcul)
// -----------------------------------------------------------------------------

TEST(FixedCouponTest, FixedCouponDoesNotPerformAnyCalculation)
{
    FixedCoupon coupon(0.04, DayCountConvention::ACT_360);

    // Le test est volontairement trivial :
    // il documente que le coupon ne calcule RIEN.
    EXPECT_DOUBLE_EQ(coupon.fixedRate(), 0.04);
}
