#include <gtest/gtest.h>
#include "time/date.hpp"

using qris::time::Date;

// -----------------------------------------------------------------------------
// Construction & invariants
// -----------------------------------------------------------------------------

TEST(DateConstruction, ValidDates)
{
    EXPECT_NO_THROW(Date(1, 1, 1900));
    EXPECT_NO_THROW(Date(31, 12, 1900));
    EXPECT_NO_THROW(Date(29, 2, 2000)); // bissextile
    EXPECT_NO_THROW(Date(28, 2, 1900)); // non bissextile
}

TEST(DateConstruction, InvalidDates)
{
    EXPECT_THROW(Date(0, 1, 1900), std::invalid_argument);
    EXPECT_THROW(Date(32, 1, 1900), std::invalid_argument);
    EXPECT_THROW(Date(31, 4, 1900), std::invalid_argument);
    EXPECT_THROW(Date(29, 2, 1900), std::invalid_argument); // 1900 non bissextile
    EXPECT_THROW(Date(1, 13, 1900), std::invalid_argument);
    EXPECT_THROW(Date(1, 1, 1899), std::invalid_argument);
}

// -----------------------------------------------------------------------------
// Serial number <-> Date bijection
// -----------------------------------------------------------------------------

TEST(DateSerial, BaseReference)
{
    Date d(1, 1, 1900);
    EXPECT_EQ(d.dateToSerialNumber(), 1);

    Date d2(1);
    EXPECT_EQ(d2.day(), 1);
    EXPECT_EQ(d2.month(), 1);
    EXPECT_EQ(d2.year(), 1900);
}

TEST(DateSerial, EndOfYear1900)
{
    Date d(31, 12, 1900);
    EXPECT_EQ(d.dateToSerialNumber(), 365);

    Date d2(365);
    EXPECT_EQ(d2.toString(), "1900-12-31");
}

TEST(DateSerial, LeapYear2000)
{
    Date d(29, 2, 2000);
    int serial = d.dateToSerialNumber();

    Date reconstructed(serial);
    EXPECT_EQ(reconstructed, d);
}

// -----------------------------------------------------------------------------
// Comparisons
// -----------------------------------------------------------------------------

TEST(DateComparison, EqualityAndOrder)
{
    Date d1(1, 1, 1900);
    Date d2(2, 1, 1900);
    Date d3(1, 1, 1900);

    EXPECT_TRUE(d1 == d3);
    EXPECT_FALSE(d1 != d3);

    EXPECT_TRUE(d1 < d2);
    EXPECT_TRUE(d2 > d1);
    EXPECT_TRUE(d1 <= d3);
    EXPECT_TRUE(d2 >= d1);
}

TEST(DateComparison, TotalOrder)
{
    Date d1(15, 6, 2020);
    Date d2(16, 6, 2020);
    Date d3(15, 7, 2020);

    EXPECT_TRUE(d1 < d2);
    EXPECT_TRUE(d2 < d3);
    EXPECT_TRUE(d1 < d3);
}

// -----------------------------------------------------------------------------
// diff()
// -----------------------------------------------------------------------------

TEST(DateDiff, BasicProperties)
{
    Date d1(1, 1, 1900);
    Date d2(2, 1, 1900);

    EXPECT_EQ(d2.diff(d1), 1);
    EXPECT_EQ(d1.diff(d2), -1);
    EXPECT_EQ(d1.diff(d1), 0);
}

TEST(DateDiff, LeapYearEffect)
{
    Date d1(28, 2, 2000);
    Date d2(1, 3, 2000);

    EXPECT_EQ(d2.diff(d1), 2); // 29 février existe
}

// -----------------------------------------------------------------------------
// addDays()
// -----------------------------------------------------------------------------

TEST(DateAddDays, SimpleAdd)
{
    Date d(1, 1, 1900);
    Date d2 = d.addDays(1);

    EXPECT_EQ(d2.toString(), "1900-01-02");
}

TEST(DateAddDays, MonthBoundary)
{
    Date d(31, 1, 2024);
    Date d2 = d.addDays(1);

    EXPECT_EQ(d2.toString(), "2024-02-01");
}

TEST(DateAddDays, YearBoundary)
{
    Date d(31, 12, 2023);
    Date d2 = d.addDays(1);

    EXPECT_EQ(d2.toString(), "2024-01-01");
}

TEST(DateAddDays, LeapYearBoundary)
{
    Date d(28, 2, 2020);
    Date d2 = d.addDays(1);
    Date d3 = d.addDays(2);

    EXPECT_EQ(d2.toString(), "2020-02-29");
    EXPECT_EQ(d3.toString(), "2020-03-01");
}

// -----------------------------------------------------------------------------
// toString()
// -----------------------------------------------------------------------------

TEST(DateToString, FormatISO)
{
    Date d(3, 7, 2024);
    EXPECT_EQ(d.toString(), "2024-07-03");
}
