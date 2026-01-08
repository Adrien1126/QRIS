#include <gtest/gtest.h>

#include "time/scheduleBuilder.hpp"
#include "time/period.hpp"
#include "time/stubRule.hpp"
#include "time/date.hpp"

using namespace qris::time;

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------

static Date D(int d, int m, int y)
{
    return Date(d, m, y);
}

// -----------------------------------------------------------------------------
// Tests — Existence et invariants structurels
// -----------------------------------------------------------------------------

TEST(ScheduleTest, ScheduleIsCreatedSuccessfully)
{
    ScheduleBuilder builder(
        D(1, 1, 2024),
        D(1, 4, 2024),
        Period(1, PeriodUnit::M),
        StubRule::ShortBack);

    EXPECT_NO_THROW(builder.build());
}

TEST(ScheduleTest, ScheduleIsNeverEmpty)
{
    ScheduleBuilder builder(
        D(1, 1, 2024),
        D(10, 1, 2024),
        Period(1, PeriodUnit::M),
        StubRule::ShortBack);

    Schedule s = builder.build();

    EXPECT_GE(s.size(), 1u);
}

// -----------------------------------------------------------------------------
// Tests — Invariants internes (indirects)
// -----------------------------------------------------------------------------

TEST(ScheduleTest, AllAccrualPeriodsHavePositiveLength)
{
    ScheduleBuilder builder(
        D(1, 1, 2024),
        D(1, 5, 2024),
        Period(1, PeriodUnit::M),
        StubRule::ShortBack);

    Schedule s = builder.build();
    const auto& accruals = s.accruals();

    for (const auto& [start, end] : accruals) {
        EXPECT_LT(start, end);
    }
}

TEST(ScheduleTest, AccrualPeriodsAreContiguous)
{
    ScheduleBuilder builder(
        D(1, 1, 2024),
        D(1, 6, 2024),
        Period(1, PeriodUnit::M),
        StubRule::ShortBack);

    Schedule s = builder.build();
    const auto& accruals = s.accruals();

    ASSERT_GE(accruals.size(), 1u);

    for (std::size_t i = 1; i < accruals.size(); ++i) {
        EXPECT_EQ(accruals[i - 1].second, accruals[i].first);
    }
}

// -----------------------------------------------------------------------------
// Tests — Robustesse sur cas limites
// -----------------------------------------------------------------------------

TEST(ScheduleTest, SingleAccrualScheduleIsValid)
{
    ScheduleBuilder builder(
        D(1, 1, 2024),
        D(15, 1, 2024),
        Period(1, PeriodUnit::Y),
        StubRule::ShortBack);

    Schedule s = builder.build();

    EXPECT_EQ(s.size(), 1u);
}

TEST(ScheduleTest, LargePeriodStillProducesValidSchedule)
{
    ScheduleBuilder builder(
        D(1, 1, 2024),
        D(1, 12, 2024),
        Period(10, PeriodUnit::Y),
        StubRule::LongBack);

    Schedule s = builder.build();

    EXPECT_EQ(s.size(), 1u);
}

// -----------------------------------------------------------------------------
// Tests — Ancrage contractuel
// -----------------------------------------------------------------------------

TEST(ScheduleTest, ScheduleCoversExactContractInterval)
{
    Date start = D(1, 1, 2024);
    Date end   = D(1, 7, 2024);

    ScheduleBuilder builder(
        start,
        end,
        Period(1, PeriodUnit::M),
        StubRule::ShortBack);

    Schedule s = builder.build();
    const auto& accruals = s.accruals();

    EXPECT_EQ(accruals.front().first, start);
    EXPECT_EQ(accruals.back().second, end);
}
