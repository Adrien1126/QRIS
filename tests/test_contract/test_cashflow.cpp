#include <gtest/gtest.h>

#include "contract/cashflow.hpp"
#include "contract/coupon/fixedCoupon.hpp"
#include "contract/coupon/floatingCoupon.hpp"

using namespace qris::contract;
using qris::time::Date;
using qris::time::DayCountConvention;
using qris::core::Currency;
using qris::core::PayReceive;
using qris::core::RateIndex;
using qris::time::Period; 
using qris::time::PeriodUnit; 
using qris::time::Calendar; 

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------

static Currency EUR()
{
    return Currency("EUR");
}

static std::shared_ptr<const RateIndex> makeEuribor3M()
{
    return std::make_shared<const RateIndex>(
        "EURIBOR_3M",
        Currency("EUR"),
        Period(3, PeriodUnit::M),
        2,
        qris::time::DayCountConvention::ACT_360,
        Calendar("TARGET", {})
    );
}

// -----------------------------------------------------------------------------
// Tests — Cashflow valide
// -----------------------------------------------------------------------------

TEST(CashflowTest, FixedCashflowValid)
{
    auto coupon = std::make_unique<FixedCoupon>(
        0.02,
        DayCountConvention::ACT_360);

    Cashflow cf(
        Date(30, 6, 2025),      // payment date
        std::nullopt,
        Date(30, 3, 2025),      // accrual start
        Date(30, 6, 2025),      // accrual end
        EUR(),
        1'000'000.0,
        PayReceive::Receive,
        std::move(coupon));

    EXPECT_EQ(cf.paymentDate(), Date(30, 6, 2025));
    EXPECT_TRUE(cf.accrualStartDate().has_value());
    EXPECT_TRUE(cf.accrualEndDate().has_value());
    EXPECT_FALSE(cf.fixingDate().has_value());

    EXPECT_EQ(cf.currency(), EUR());
    EXPECT_EQ(cf.notional(), 1'000'000.0);
    EXPECT_EQ(cf.direction(), PayReceive::Receive);

    EXPECT_EQ(cf.coupon().type(), CouponType::Fixed);
    EXPECT_EQ(cf.coupon().dayCount(), DayCountConvention::ACT_360);
}

TEST(CashflowTest, FloatingCashflowValid)
{
    auto coupon = std::make_unique<FloatingCoupon>(
        makeEuribor3M(),
        0.001,
        DayCountConvention::ACT_360);

    Cashflow cf(
        Date(30, 9, 2025),      // payment date
        Date(27, 6, 2025),      // fixing
        Date(30, 6, 2025),      // accrual start
        Date(30, 9, 2025),      // accrual end
        EUR(),
        5'000'000.0,
        PayReceive::Pay,
        std::move(coupon));

    EXPECT_TRUE(cf.fixingDate().has_value());
    EXPECT_LE(*cf.fixingDate(), *cf.accrualStartDate());

    EXPECT_EQ(cf.coupon().type(), CouponType::Floating);
    EXPECT_EQ(cf.coupon().dayCount(), DayCountConvention::ACT_360);
}

// -----------------------------------------------------------------------------
// Tests — Invariants violés
// -----------------------------------------------------------------------------

TEST(CashflowTest, NullCouponThrows)
{
    EXPECT_THROW(
        Cashflow(
            Date(30, 6, 2025),
            std::nullopt,
            Date(30, 3, 2025),
            Date(30, 6, 2025),
            EUR(),
            1'000'000.0,
            PayReceive::Receive,
            nullptr),
        std::invalid_argument);
}

TEST(CashflowTest, NegativeNotionalThrows)
{
    auto coupon = std::make_unique<FixedCoupon>(
        0.02,
        DayCountConvention::ACT_360);

    EXPECT_THROW(
        Cashflow(
            Date(30, 6, 2025),
            std::nullopt,
            Date(30, 3, 2025),
            Date(30, 6, 2025),
            EUR(),
            -1'000'000.0,
            PayReceive::Receive,
            std::move(coupon)),
        std::invalid_argument);
}

TEST(CashflowTest, AccrualEndBeforeStartThrows)
{
    auto coupon = std::make_unique<FixedCoupon>(
        0.02,
        DayCountConvention::ACT_360);

    EXPECT_THROW(
        Cashflow(
            Date(30, 6, 2025),
            std::nullopt,
            Date(30, 6, 2025),
            Date(30, 3, 2025),   // end < start
            EUR(),
            1'000'000.0,
            PayReceive::Receive,
            std::move(coupon)),
        std::invalid_argument);
}

TEST(CashflowTest, FixingAfterAccrualStartThrows)
{
    auto coupon = std::make_unique<FloatingCoupon>(
        makeEuribor3M(),
        0.001,
        DayCountConvention::ACT_360);

    EXPECT_THROW(
        Cashflow(
            Date(30, 9, 2025),
            Date(1, 7, 2025),    // fixing > accrual start
            Date(30, 6, 2025),
            Date(30, 9, 2025),
            EUR(),
            1'000'000.0,
            PayReceive::Pay,
            std::move(coupon)),
        std::invalid_argument);
}

TEST(CashflowTest, TemporalGettersReturnExactValues)
{
    auto coupon = std::make_unique<FixedCoupon>(
        0.015,
        DayCountConvention::ACT_365F);

    Date payment(15, 1, 2026);
    Date accrualStart(15, 10, 2025);
    Date accrualEnd(15, 1, 2026);
    Date fixing(13, 10, 2025);

    Cashflow cf(
        payment,
        fixing,
        accrualStart,
        accrualEnd,
        EUR(),
        2'000'000.0,
        PayReceive::Receive,
        std::move(coupon));

    // Payment date
    EXPECT_EQ(cf.paymentDate(), payment);

    // Fixing date
    ASSERT_TRUE(cf.fixingDate().has_value());
    EXPECT_EQ(*cf.fixingDate(), fixing);

    // Accrual dates
    ASSERT_TRUE(cf.accrualStartDate().has_value());
    ASSERT_TRUE(cf.accrualEndDate().has_value());
    EXPECT_EQ(*cf.accrualStartDate(), accrualStart);
    EXPECT_EQ(*cf.accrualEndDate(), accrualEnd);
}

TEST(CashflowTest, OptionalDatesAreEmptyWhenNotProvided)
{
    auto coupon = std::make_unique<FixedCoupon>(
        0.01,
        DayCountConvention::ACT_360);

    Cashflow cf(
        Date(31, 12, 2025),
        std::nullopt,
        std::nullopt,
        std::nullopt,
        EUR(),
        500'000.0,
        PayReceive::Pay,
        std::move(coupon));

    EXPECT_FALSE(cf.fixingDate().has_value());
    EXPECT_FALSE(cf.accrualStartDate().has_value());
    EXPECT_FALSE(cf.accrualEndDate().has_value());
}

TEST(CashflowTest, FinancialGettersReturnExactValues)
{
    auto coupon = std::make_unique<FloatingCoupon>(
        makeEuribor3M(),
        0.0025,
        DayCountConvention::ACT_360);

    Currency currency = EUR();
    double notional = 10'000'000.0;
    PayReceive direction = PayReceive::Pay;

    Cashflow cf(
        Date(30, 6, 2026),
        std::nullopt,
        Date(30, 12, 2025),
        Date(30, 6, 2026),
        currency,
        notional,
        direction,
        std::move(coupon));

    EXPECT_EQ(cf.currency(), currency);
    EXPECT_EQ(cf.notional(), notional);
    EXPECT_EQ(cf.direction(), direction);
}

TEST(CashflowTest, CouponGetterReturnsCorrectPolymorphicType)
{
    auto coupon = std::make_unique<FloatingCoupon>(
        makeEuribor3M(),
        0.001,
        DayCountConvention::ACT_360);

    const FloatingCoupon* rawPtr = coupon.get();

    Cashflow cf(
        Date(30, 9, 2025),
        std::nullopt,
        Date(30, 6, 2025),
        Date(30, 9, 2025),
        EUR(),
        1'000'000.0,
        PayReceive::Receive,
        std::move(coupon));

    const Coupon& c = cf.coupon();

    // Polymorphisme
    EXPECT_EQ(c.type(), CouponType::Floating);

    // Même objet sous-jacent
    EXPECT_EQ(&c, rawPtr);
}
