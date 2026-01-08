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
// Tests — Construction invalide
// -----------------------------------------------------------------------------

TEST(ScheduleBuilderTest, StartDateAfterEndDateThrows)
{
    EXPECT_THROW(
        ScheduleBuilder(
            D(1, 7, 2024),
            D(1, 1, 2024),
            Period(1, PeriodUnit::M),
            StubRule::ShortBack),
        std::invalid_argument);
}

TEST(ScheduleBuilderTest, StartDateEqualsEndDateThrows)
{
    EXPECT_THROW(
        ScheduleBuilder(
            D(1, 1, 2024),
            D(1, 1, 2024),
            Period(1, PeriodUnit::M),
            StubRule::ShortBack),
        std::invalid_argument);
}

// -----------------------------------------------------------------------------
// Tests — Génération forward (stub back)
// -----------------------------------------------------------------------------

TEST(ScheduleBuilderTest, ForwardShortBackNominal)
{
    ScheduleBuilder builder(
        D(1, 1, 2024),
        D(1, 7, 2024),
        Period(1, PeriodUnit::M),
        StubRule::ShortBack);

    Schedule s = builder.build();
    const auto& accruals = s.accruals();

    ASSERT_EQ(accruals.size(), 6u);

    EXPECT_EQ(accruals.front().first,  D(1, 1, 2024));
    EXPECT_EQ(accruals.front().second, D(1, 2, 2024));

    EXPECT_EQ(accruals.back().first,  D(1, 6, 2024));
    EXPECT_EQ(accruals.back().second, D(1, 7, 2024));
}

TEST(ScheduleBuilderTest, ForwardLongBack)
{
    ScheduleBuilder builder(
        D(1, 1, 2024),
        D(15, 6, 2024),
        Period(1, PeriodUnit::M),
        StubRule::LongBack);

    Schedule s = builder.build();
    const auto& accruals = s.accruals();

    ASSERT_GE(accruals.size(), 1u);
    EXPECT_EQ(accruals.back().second, D(15, 6, 2024));
}

// -----------------------------------------------------------------------------
// Tests — Génération backward (stub front)
// -----------------------------------------------------------------------------

TEST(ScheduleBuilderTest, BackwardShortFrontNominal)
{
    ScheduleBuilder builder(
        D(1, 1, 2024),
        D(1, 7, 2024),
        Period(1, PeriodUnit::M),
        StubRule::ShortFront);

    Schedule s = builder.build();
    const auto& accruals = s.accruals();

    ASSERT_EQ(accruals.size(), 6u);

    EXPECT_EQ(accruals.front().first,  D(1, 1, 2024));
    EXPECT_EQ(accruals.front().second, D(1, 2, 2024));

    EXPECT_EQ(accruals.back().first,  D(1, 6, 2024));
    EXPECT_EQ(accruals.back().second, D(1, 7, 2024));
}

TEST(ScheduleBuilderTest, BackwardLongFront)
{
    ScheduleBuilder builder(
        D(10, 1, 2024),
        D(1, 7, 2024),
        Period(1, PeriodUnit::M),
        StubRule::LongFront);

    Schedule s = builder.build();
    const auto& accruals = s.accruals();

    ASSERT_GE(accruals.size(), 1u);
    EXPECT_EQ(accruals.front().first, D(10, 1, 2024));
}

// -----------------------------------------------------------------------------
// Tests — Cas limites (nouvelle règle : jamais vide)
// -----------------------------------------------------------------------------

TEST(ScheduleBuilderTest, PeriodLongerThanTotalDurationCreatesSingleAccrual)
{
    ScheduleBuilder builder(
        D(1, 1, 2024),
        D(1, 6, 2024),
        Period(1, PeriodUnit::Y),
        StubRule::ShortBack);

    Schedule s = builder.build();
    const auto& accruals = s.accruals();

    ASSERT_EQ(accruals.size(), 1u);
    EXPECT_EQ(accruals.front().first,  D(1, 1, 2024));
    EXPECT_EQ(accruals.front().second, D(1, 6, 2024));
}

TEST(ScheduleBuilderTest, SingleAccrualWithShortPeriod)
{
    ScheduleBuilder builder(
        D(1, 1, 2024),
        D(15, 1, 2024),
        Period(1, PeriodUnit::M),
        StubRule::ShortBack);

    Schedule s = builder.build();
    const auto& accruals = s.accruals();

    ASSERT_EQ(accruals.size(), 1u);
    EXPECT_EQ(accruals.front().first,  D(1, 1, 2024));
    EXPECT_EQ(accruals.front().second, D(15, 1, 2024));
}

// -----------------------------------------------------------------------------
// Tests — Continuité et ordre
// -----------------------------------------------------------------------------

TEST(ScheduleBuilderTest, AccrualsAreContiguousAndOrdered)
{
    ScheduleBuilder builder(
        D(1, 1, 2024),
        D(1, 5, 2024),
        Period(1, PeriodUnit::M),
        StubRule::ShortBack);

    Schedule s = builder.build();
    const auto& accruals = s.accruals();

    ASSERT_GE(accruals.size(), 1u);

    for (std::size_t i = 1; i < accruals.size(); ++i) {
        EXPECT_EQ(accruals[i - 1].second, accruals[i].first);
        EXPECT_LT(accruals[i].first, accruals[i].second);
    }
}
