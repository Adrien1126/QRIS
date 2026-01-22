#include <gtest/gtest.h>

#include "contract/coupon/floatingCoupon.hpp"
#include "time/dayCountConvention.hpp"

using qris::contract::FloatingCoupon;
using qris::contract::CouponType;
using qris::time::DayCountConvention;
using qris::time::Period;
using qris::time::PeriodUnit;
using qris::time::Calendar;
using qris::core::Currency;
using qris::core::RateIndex;

static std::shared_ptr<const RateIndex> makeEuribor3M()
{
    return std::make_shared<const RateIndex>(
        "EURIBOR_3M",
        Currency("EUR"),
        Period(3, PeriodUnit::M),
        2,
        DayCountConvention::ACT_360,
        Calendar("TARGET", {})
    );
}

// -----------------------------------------------------------------------------
// Construction valide
// -----------------------------------------------------------------------------

TEST(FloatingCouponTest, ValidConstruction)
{
    auto index = makeEuribor3M();

    FloatingCoupon coupon(
        index,
        0.0015,
        DayCountConvention::ACT_360
    );

    EXPECT_EQ(coupon.type(), CouponType::Floating);
    EXPECT_EQ(coupon.index().code(), "EURIBOR_3M");
    EXPECT_DOUBLE_EQ(coupon.spread(), 0.0015);
    EXPECT_EQ(coupon.dayCount(), DayCountConvention::ACT_360);
}

TEST(FloatingCouponTest, ZeroSpreadIsAllowed)
{
    FloatingCoupon coupon(
        makeEuribor3M(),
        0.0,
        DayCountConvention::ACT_365F
    );

    EXPECT_DOUBLE_EQ(coupon.spread(), 0.0);
}

TEST(FloatingCouponTest, NegativeSpreadIsAllowed)
{
    FloatingCoupon coupon(
        makeEuribor3M(),
        -0.0005,
        DayCountConvention::ACT_360
    );

    EXPECT_DOUBLE_EQ(coupon.spread(), -0.0005);
}

// -----------------------------------------------------------------------------
// Validation de l’index
// -----------------------------------------------------------------------------

TEST(FloatingCouponTest, NullIndexThrows)
{
    EXPECT_THROW(
        FloatingCoupon(nullptr, 0.01, DayCountConvention::ACT_360),
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
            makeEuribor3M(),
            std::numeric_limits<double>::quiet_NaN(),
            DayCountConvention::ACT_360),
        std::invalid_argument
    );
}

TEST(FloatingCouponTest, InfiniteSpreadThrows)
{
    EXPECT_THROW(
        FloatingCoupon(
            makeEuribor3M(),
            std::numeric_limits<double>::infinity(),
            DayCountConvention::ACT_360),
        std::invalid_argument
    );
}

// -----------------------------------------------------------------------------
// Validation de la DayCountConvention
// -----------------------------------------------------------------------------

TEST(FloatingCouponTest, DayCountIsIndependentFromIndex)
{
    FloatingCoupon coupon(
        makeEuribor3M(),
        0.01,
        DayCountConvention::ACT_365F
    );

    EXPECT_EQ(coupon.dayCount(), DayCountConvention::ACT_365F);
}
