#include <gtest/gtest.h>

#include "time/date.hpp"
#include "time/dayCountConvention.hpp"

using namespace qris::time;

// -----------------------------------------------------------------------------
// Fixture de base
// -----------------------------------------------------------------------------

class DayCountConventionTest : public ::testing::Test {
protected:
    Date d1;
    Date d2;

    DayCountConventionTest()
        : d1(1, 1, 2024),
          d2(31, 1, 2024)
    {}
};

// -----------------------------------------------------------------------------
// ACT/360
// -----------------------------------------------------------------------------

TEST_F(DayCountConventionTest, ACT360_BasicDifference) {
    double yf = yearFraction(d1, d2, DayCountConvention::ACT_360);

    EXPECT_DOUBLE_EQ(yf, 30.0 / 360.0);
}

TEST_F(DayCountConventionTest, ACT360_NegativeFraction) {
    double yf = yearFraction(d2, d1, DayCountConvention::ACT_360);

    EXPECT_DOUBLE_EQ(yf, -30.0 / 360.0);
}

// -----------------------------------------------------------------------------
// ACT/365F
// -----------------------------------------------------------------------------

TEST_F(DayCountConventionTest, ACT365F_BasicDifference) {
    double yf = yearFraction(d1, d2, DayCountConvention::ACT_365F);

    EXPECT_DOUBLE_EQ(yf, 30.0 / 365.0);
}

TEST_F(DayCountConventionTest, ACT365F_OneYear) {
    Date start(1, 1, 2023);
    Date end(1, 1, 2024);

    double yf = yearFraction(start, end, DayCountConvention::ACT_365F);

    EXPECT_DOUBLE_EQ(yf, 365.0 / 365.0);
}

// -----------------------------------------------------------------------------
// 30E/360 (Eurobond)
// -----------------------------------------------------------------------------

TEST(DayCount30E360Test, EurobondExample) {
    // Exemple standard :
    // 28/02/2024 -> 31/03/2024
    Date start(28, 2, 2024);
    Date end(31, 3, 2024);

    double yf = yearFraction(start, end, DayCountConvention::THIRTY_E_360);

    // d1 = min(28,30) = 28
    // d2 = min(31,30) = 30
    // days360 = 30*(3-2) + (30-28) = 30 + 2 = 32
    EXPECT_DOUBLE_EQ(yf, 32.0 / 360.0);
}

TEST(DayCount30E360Test, Eurobond_EndOfMonth) {
    Date start(31, 1, 2024);
    Date end(28, 2, 2024);

    double yf = yearFraction(start, end, DayCountConvention::THIRTY_E_360);

    // d1 = 30, d2 = 28
    // days360 = 30*(2-1) + (28-30) = 30 - 2 = 28
    EXPECT_DOUBLE_EQ(yf, 28.0 / 360.0);
}

// -----------------------------------------------------------------------------
// 30/360 US (Bond Basis / Brigo)
// -----------------------------------------------------------------------------

TEST(DayCount30US360Test, BrigoExampleFromBook) {
    // Exemple explicitement cité par Brigo & Mercurio
    // January 4, 2000 -> July 4, 2000
    Date start(4, 1, 2000);
    Date end(4, 7, 2000);

    double yf = yearFraction(start, end, DayCountConvention::THIRTY_360_US);

    EXPECT_DOUBLE_EQ(yf, 0.5);
}

TEST(DayCount30US360Test, Brigo_EndOfMonth) {
    Date start(31, 1, 2024);
    Date end(28, 2, 2024);

    double yf = yearFraction(start, end, DayCountConvention::THIRTY_360_US);

    // dayPart = max(30-31,0) + min(28,30) = 0 + 28 = 28
    // monthPart = 30*(2-1-1) = 0
    // yearPart = 0
    EXPECT_DOUBLE_EQ(yf, 28.0 / 360.0);
}

// -----------------------------------------------------------------------------
// Robustesse
// -----------------------------------------------------------------------------

TEST(DayCountConventionRobustnessTest, ZeroLengthPeriod) {
    Date d(15, 6, 2024);

    double yf = yearFraction(d, d, DayCountConvention::ACT_360);

    EXPECT_DOUBLE_EQ(yf, 0.0);
}
