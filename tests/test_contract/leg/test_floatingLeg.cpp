#include <gtest/gtest.h>

#include "contract/leg/floatingLeg.hpp"
#include "contract/cashflow.hpp"

#include "time/date.hpp"
#include "time/period.hpp"
#include "time/calendar.hpp"
#include "time/stubRule.hpp"
#include "time/businessDayConvention.hpp"

#include "core/types/rateIndex.hpp"
#include "core/types/currency.hpp"
#include "core/types/payReceive.hpp"

using namespace qris::contract;
using namespace qris::time;
using namespace qris::core;

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------

static Currency EUR()
{
    return Currency("EUR");
}

static Calendar TARGET()
{
    return Calendar("TARGET", {});
}

static std::shared_ptr<const RateIndex> EURIBOR_3M()
{
    return std::make_shared<RateIndex>(
        "EURIBOR_3M",
        EUR(),
        Period(3, PeriodUnit::M),
        2,                              // fixing lag (business days)
        DayCountConvention::ACT_360,
        TARGET()
    );
}

// -----------------------------------------------------------------------------
// Construction & validation
// -----------------------------------------------------------------------------

TEST(FloatingLegTest, ConstructsSuccessfully)
{
    EXPECT_NO_THROW(
        FloatingLeg(
            Date(30, 6, 2025),
            Date(30, 6, 2026),
            Period(3, PeriodUnit::M),
            TARGET(),
            BusinessDayConvention::ModifiedFollowing,
            StubRule::ShortBack,
            2,          // payment lag (business days)
            2,          // fixing lag (business days)
            1'000'000.0,
            EUR(),
            PayReceive::Receive,
            EURIBOR_3M(),
            0.0010,
            DayCountConvention::ACT_360
        )
    );
}

TEST(FloatingLegTest, NullIndexThrows)
{
    EXPECT_THROW(
        FloatingLeg(
            Date(1, 1, 2025),
            Date(1, 1, 2026),
            Period(3, PeriodUnit::M),
            TARGET(),
            BusinessDayConvention::Following,
            StubRule::ShortBack,
            2,
            2,
            1'000'000.0,
            EUR(),
            PayReceive::Receive,
            nullptr,
            0.0,
            DayCountConvention::ACT_360
        ),
        std::invalid_argument
    );
}

// -----------------------------------------------------------------------------
// Cashflow generation
// -----------------------------------------------------------------------------

TEST(FloatingLegTest, GeneratesExpectedNumberOfCashflows)
{
    FloatingLeg leg(
        Date(30, 6, 2025),
        Date(30, 6, 2026),
        Period(3, PeriodUnit::M),
        TARGET(),
        BusinessDayConvention::Following,
        StubRule::ShortBack,
        2,
        2,
        1'000'000.0,
        EUR(),
        PayReceive::Receive,
        EURIBOR_3M(),
        0.0,
        DayCountConvention::ACT_360
    );

    EXPECT_EQ(leg.cashflows().size(), 4);
}

TEST(FloatingLegTest, CashflowsAreOrderedByPaymentDate)
{
    FloatingLeg leg(
        Date(30, 6, 2025),
        Date(30, 6, 2026),
        Period(3, PeriodUnit::M),
        TARGET(),
        BusinessDayConvention::Following,
        StubRule::ShortBack,
        2,
        2,
        1'000'000.0,
        EUR(),
        PayReceive::Receive,
        EURIBOR_3M(),
        0.0,
        DayCountConvention::ACT_360
    );

    const auto& cfs = leg.cashflows();

    for (std::size_t i = 1; i < cfs.size(); ++i) {
        EXPECT_LT(cfs[i - 1].paymentDate(), cfs[i].paymentDate());
    }
}

// -----------------------------------------------------------------------------
// Fixing logic
// -----------------------------------------------------------------------------

TEST(FloatingLegTest, FixingDateIsBeforeAccrualStart)
{
    FloatingLeg leg(
        Date(30, 6, 2025),
        Date(30, 9, 2025),
        Period(3, PeriodUnit::M),
        TARGET(),
        BusinessDayConvention::Following,
        StubRule::ShortBack,
        2,
        2,
        1'000'000.0,
        EUR(),
        PayReceive::Receive,
        EURIBOR_3M(),
        0.0,
        DayCountConvention::ACT_360
    );

    const auto& cf = leg.cashflows().front();

    ASSERT_TRUE(cf.fixingDate().has_value());
    EXPECT_LT(cf.fixingDate().value(), cf.accrualStartDate().value());
}

// -----------------------------------------------------------------------------
// Coupon semantics
// -----------------------------------------------------------------------------

TEST(FloatingLegTest, CouponIsSharedAcrossCashflows)
{
    FloatingLeg leg(
        Date(30, 6, 2025),
        Date(30, 6, 2026),
        Period(3, PeriodUnit::M),
        TARGET(),
        BusinessDayConvention::Following,
        StubRule::ShortBack,
        2,
        2,
        1'000'000.0,
        EUR(),
        PayReceive::Receive,
        EURIBOR_3M(),
        0.0,
        DayCountConvention::ACT_360
    );

    const auto& cfs = leg.cashflows();
    const Coupon* ref = &cfs.front().coupon();

    for (const auto& cf : cfs) {
        EXPECT_EQ(&cf.coupon(), ref);
    }
}

// -----------------------------------------------------------------------------
// Temporal consistency
// -----------------------------------------------------------------------------

TEST(FloatingLegTest, AccrualPeriodsAreContiguous)
{
    FloatingLeg leg(
        Date(30, 6, 2025),
        Date(30, 6, 2026),
        Period(3, PeriodUnit::M),
        TARGET(),
        BusinessDayConvention::Following,
        StubRule::ShortBack,
        2,
        2,
        1'000'000.0,
        EUR(),
        PayReceive::Receive,
        EURIBOR_3M(),
        0.0,
        DayCountConvention::ACT_360
    );

    const auto& cfs = leg.cashflows();

    for (std::size_t i = 1; i < cfs.size(); ++i) {
        EXPECT_EQ(
            cfs[i - 1].accrualEndDate().value(),
            cfs[i].accrualStartDate().value()
        );
    }
}
