#include <gtest/gtest.h>

#include "time/date.hpp"
#include "time/period.hpp"
#include "time/time_arithmetic.hpp"

using qris::time::Date;
using qris::time::Period;
using qris::time::PeriodUnit;
using qris::time::addPeriod;
using qris::time::subtractPeriod;

// -----------------------------------------------------------------------------
// Days (D)
// -----------------------------------------------------------------------------

TEST(TimeArithmeticTest, AddDays)
{
    Date d(15, 3, 2025);
    Period p(10, PeriodUnit::D);

    Date result = addPeriod(d, p);
    EXPECT_EQ(result, Date(25, 3, 2025));
}

TEST(TimeArithmeticTest, SubtractDays)
{
    Date d(15, 3, 2025);
    Period p(10, PeriodUnit::D);

    Date result = subtractPeriod(d, p);
    EXPECT_EQ(result, Date(5, 3, 2025));
}

// -----------------------------------------------------------------------------
// Weeks (W)
// -----------------------------------------------------------------------------

TEST(TimeArithmeticTest, AddWeeks)
{
    Date d(1, 1, 2025);
    Period p(2, PeriodUnit::W);

    Date result = addPeriod(d, p);
    EXPECT_EQ(result, Date(15, 1, 2025));
}

TEST(TimeArithmeticTest, SubtractWeeks)
{
    Date d(15, 1, 2025);
    Period p(2, PeriodUnit::W);

    Date result = subtractPeriod(d, p);
    EXPECT_EQ(result, Date(1, 1, 2025));
}

// -----------------------------------------------------------------------------
// Months (M) — standard cases
// -----------------------------------------------------------------------------

TEST(TimeArithmeticTest, AddMonthsSimple)
{
    Date d(15, 1, 2025);
    Period p(2, PeriodUnit::M);

    Date result = addPeriod(d, p);
    EXPECT_EQ(result, Date(15, 3, 2025));
}

TEST(TimeArithmeticTest, SubtractMonthsSimple)
{
    Date d(15, 3, 2025);
    Period p(2, PeriodUnit::M);

    Date result = subtractPeriod(d, p);
    EXPECT_EQ(result, Date(15, 1, 2025));
}

// -----------------------------------------------------------------------------
// Months (M) — end-of-month clamping
// -----------------------------------------------------------------------------

TEST(TimeArithmeticTest, AddMonth_EndOfJanuary)
{
    Date d(31, 1, 2025);
    Period p(1, PeriodUnit::M);

    Date result = addPeriod(d, p);
    EXPECT_EQ(result, Date(28, 2, 2025));
}

TEST(TimeArithmeticTest, AddMonth_EndOfJanuary_LeapYear)
{
    Date d(31, 1, 2024);
    Period p(1, PeriodUnit::M);

    Date result = addPeriod(d, p);
    EXPECT_EQ(result, Date(29, 2, 2024));
}

TEST(TimeArithmeticTest, SubtractMonth_EndOfMarch)
{
    Date d(31, 3, 2025);
    Period p(1, PeriodUnit::M);

    Date result = subtractPeriod(d, p);
    EXPECT_EQ(result, Date(28, 2, 2025));
}

// -----------------------------------------------------------------------------
// Years (Y)
// -----------------------------------------------------------------------------

TEST(TimeArithmeticTest, AddYearsSimple)
{
    Date d(15, 6, 2025);
    Period p(3, PeriodUnit::Y);

    Date result = addPeriod(d, p);
    EXPECT_EQ(result, Date(15, 6, 2028));
}

TEST(TimeArithmeticTest, SubtractYearsSimple)
{
    Date d(15, 6, 2025);
    Period p(3, PeriodUnit::Y);

    Date result = subtractPeriod(d, p);
    EXPECT_EQ(result, Date(15, 6, 2022));
}

// -----------------------------------------------------------------------------
// Years (Y) — February 29 handling
// -----------------------------------------------------------------------------

TEST(TimeArithmeticTest, AddYear_FromLeapDay)
{
    Date d(29, 2, 2024);
    Period p(1, PeriodUnit::Y);

    Date result = addPeriod(d, p);
    EXPECT_EQ(result, Date(28, 2, 2025));
}

TEST(TimeArithmeticTest, SubtractYear_FromLeapDay)
{
    Date d(29, 2, 2024);
    Period p(1, PeriodUnit::Y);

    Date result = subtractPeriod(d, p);
    EXPECT_EQ(result, Date(28, 2, 2023));
}

TEST(TimeArithmeticTest, AddYears_LeapCycle)
{
    Date d(29, 2, 2020);
    Period p(4, PeriodUnit::Y);

    Date result = addPeriod(d, p);
    EXPECT_EQ(result, Date(29, 2, 2024));
}

// -----------------------------------------------------------------------------
// Symmetry checks
// -----------------------------------------------------------------------------

TEST(TimeArithmeticTest, AddThenSubtractIsIdentity)
{
    Date d(17, 9, 2025);
    Period p1(10, PeriodUnit::D);
    Period p2(3, PeriodUnit::M);
    Period p3(2, PeriodUnit::Y);

    EXPECT_EQ(subtractPeriod(addPeriod(d, p1), p1), d);
    EXPECT_EQ(subtractPeriod(addPeriod(d, p2), p2), d);
    EXPECT_EQ(subtractPeriod(addPeriod(d, p3), p3), d);
}
