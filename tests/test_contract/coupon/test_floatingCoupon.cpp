#include <gtest/gtest.h>

#include "contract/coupon/floatingCoupon.hpp"
#include "time/dayCountConvention.hpp"

using qris::contract::FloatingCoupon;
using qris::contract::CouponType;
using qris::time::DayCountConvention;

// -----------------------------------------------------------------------------
// Construction valide
// -----------------------------------------------------------------------------

TEST(FloatingCouponTest, ValidConstruction)
{
    FloatingCoupon coupon(
        "EURIBOR_3M",
        0.0015,
        DayCountConvention::ACT_360
    );

    EXPECT_EQ(coupon.type(), CouponType::Floating);
    EXPECT_EQ(coupon.index(), "EURIBOR_3M");
    EXPECT_DOUBLE_EQ(coupon.spread(), 0.0015);
    EXPECT_EQ(coupon.dayCount(), DayCountConvention::ACT_360);
}

TEST(FloatingCouponTest, ZeroSpreadIsAllowed)
{
    FloatingCoupon coupon(
        "EURIBOR_6M",
        0.0,
        DayCountConvention::ACT_365F
    );

    EXPECT_DOUBLE_EQ(coupon.spread(), 0.0);
}

TEST(FloatingCouponTest, NegativeSpreadIsAllowed)
{
    FloatingCoupon coupon(
        "SOFR",
        -0.0005,
        DayCountConvention::ACT_360
    );

    EXPECT_DOUBLE_EQ(coupon.spread(), -0.0005);
}

// -----------------------------------------------------------------------------
// Validation de l’index
// -----------------------------------------------------------------------------

TEST(FloatingCouponTest, EmptyIndexThrows)
{
    EXPECT_THROW(
        FloatingCoupon("", 0.01, DayCountConvention::ACT_360),
        std::invalid_argument
    );
}

TEST(FloatingCouponTest, BlankIndexThrows)
{
    EXPECT_THROW(
        FloatingCoupon("   ", 0.01, DayCountConvention::ACT_360),
        std::invalid_argument
    );
}

// -----------------------------------------------------------------------------
// Validation du spread
// -----------------------------------------------------------------------------

TEST(FloatingCouponTest, NaNSpreadThrows)
{
    EXPECT_THROW(
        FloatingCoupon(
            "EURIBOR_3M",
            std::numeric_limits<double>::quiet_NaN(),
            DayCountConvention::ACT_360),
        std::invalid_argument
    );
}

TEST(FloatingCouponTest, InfiniteSpreadThrows)
{
    EXPECT_THROW(
        FloatingCoupon(
            "EURIBOR_3M",
            std::numeric_limits<double>::infinity(),
            DayCountConvention::ACT_360),
        std::invalid_argument
    );
}

// -----------------------------------------------------------------------------
// Validation de la DayCountConvention
// -----------------------------------------------------------------------------

TEST(FloatingCouponTest, SupportedDayCountConventions)
{
    EXPECT_NO_THROW(
        FloatingCoupon("EURIBOR_3M", 0.01, DayCountConvention::ACT_360));

    EXPECT_NO_THROW(
        FloatingCoupon("EURIBOR_3M", 0.01, DayCountConvention::ACT_365F));

    EXPECT_NO_THROW(
        FloatingCoupon("EURIBOR_3M", 0.01, DayCountConvention::THIRTY_360_US));

    EXPECT_NO_THROW(
        FloatingCoupon("EURIBOR_3M", 0.01, DayCountConvention::THIRTY_E_360));
}
