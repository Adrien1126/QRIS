#include <gtest/gtest.h>

#include "contract/leg/fixedLeg.hpp"

#include "time/date.hpp"
#include "time/period.hpp"
#include "time/calendar.hpp"
#include "time/businessDayConvention.hpp"
#include "time/stubRule.hpp"

using namespace qris::contract;
using namespace qris::time;

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------

static Date D(int d, int m, int y) {
    return Date(d, m, y);
}

static Calendar targetCalendar() {
    return Calendar(
        "TARGET",
        {} // pas de jours fériés pour les tests
    );
}

static qris::core::Currency EUR() {
    return qris::core::Currency("EUR");
}


TEST(FixedLegTest, GeneratesExpectedNumberOfCashflows)
{
    FixedLeg leg(
        D(1, 1, 2024),
        D(1, 1, 2025),
        Period(3, PeriodUnit::M),
        targetCalendar(),
        BusinessDayConvention::ModifiedFollowing,
        StubRule::ShortBack,
        2,
        1'000'000.0,
        EUR(),
        qris::core::PayReceive::Receive,
        0.02,
        DayCountConvention::ACT_360
    );

    ASSERT_EQ(leg.cashflows().size(), 4);
}

TEST(FixedLegTest, AccrualPeriodsAreContiguous)
{
    FixedLeg leg(
        D(1, 1, 2024),
        D(1, 1, 2025),
        Period(3, PeriodUnit::M),
        targetCalendar(),
        BusinessDayConvention::ModifiedFollowing,
        StubRule::ShortBack,
        2,
        1'000'000.0,
        EUR(),
        qris::core::PayReceive::Receive,
        0.02,
        DayCountConvention::ACT_360
    );

    const auto& cfs = leg.cashflows();

    for (std::size_t i = 1; i < cfs.size(); ++i) {
        EXPECT_EQ(
            *cfs[i - 1].accrualEndDate(),
            *cfs[i].accrualStartDate()
        );
    }
}

TEST(FixedLegTest, PaymentDatesAreStrictlyIncreasing)
{
    FixedLeg leg(
        D(1, 1, 2024),
        D(1, 1, 2025),
        Period(3, PeriodUnit::M),
        targetCalendar(),
        BusinessDayConvention::ModifiedFollowing,
        StubRule::ShortBack,
        2,
        1'000'000.0,
        EUR(),
        qris::core::PayReceive::Receive,
        0.02,
        DayCountConvention::ACT_360
    );

    const auto& cfs = leg.cashflows();

    for (std::size_t i = 1; i < cfs.size(); ++i) {
        EXPECT_LT(cfs[i - 1].paymentDate(), cfs[i].paymentDate());
    }
}

TEST(FixedLegTest, CouponIsSharedAcrossCashflows)
{
    FixedLeg leg(
        D(1, 1, 2024),
        D(1, 1, 2025),
        Period(6, PeriodUnit::M),
        targetCalendar(),
        BusinessDayConvention::ModifiedFollowing,
        StubRule::ShortBack,
        2,
        1'000'000.0,
        EUR(),
        qris::core::PayReceive::Receive,
        0.03,
        DayCountConvention::ACT_360
    );

    const auto& cfs = leg.cashflows();

    const Coupon* ref = &cfs.front().coupon();
    for (const auto& cf : cfs) {
        EXPECT_EQ(ref, &cf.coupon());
    }
}

TEST(FixedLegTest, InvalidDatesThrow)
{
    EXPECT_THROW(
        FixedLeg(
            D(1, 1, 2025),
            D(1, 1, 2024),
            Period(3, PeriodUnit::M),
            targetCalendar(),
            BusinessDayConvention::ModifiedFollowing,
            StubRule::ShortBack,
            2,
            1'000'000.0,
            EUR(),
            qris::core::PayReceive::Receive,
            0.02,
            DayCountConvention::ACT_360
        ),
        std::invalid_argument
    );
}

