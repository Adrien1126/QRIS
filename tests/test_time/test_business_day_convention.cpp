#include <gtest/gtest.h>

#include "time/date.hpp"
#include "time/calendar.hpp"
#include "time/businessDayConvention.hpp"

using namespace qris::time;

// -----------------------------------------------------------------------------
// Fixture de test
// -----------------------------------------------------------------------------

class BusinessDayConventionTest : public ::testing::Test {
protected:
    Calendar calendar;

    BusinessDayConventionTest()
        : calendar(
              "TARGET",
              {
                  // Quelques jours fériés fixes pour les tests
                  Date(1, 1, 2024),   // Nouvel An
                  Date(1, 5, 2024),   // Fête du travail
                  Date(25, 12, 2024)  // Noël
              })
    {}
};

// -----------------------------------------------------------------------------
// Tests généraux
// -----------------------------------------------------------------------------

TEST_F(BusinessDayConventionTest, AlreadyBusinessDay_ReturnsSameDate) {
    Date d(3, 4, 2024); // Mercredi

    Date adjusted = adjustDate(d, calendar, BusinessDayConvention::ModifiedFollowing);

    EXPECT_EQ(adjusted, d);
}

TEST_F(BusinessDayConventionTest, Unadjusted_ReturnsSameDateEvenIfWeekend) {
    Date d(6, 4, 2024); // Samedi

    Date adjusted = adjustDate(d, calendar, BusinessDayConvention::Unadjusted);

    EXPECT_EQ(adjusted, d);
}

// -----------------------------------------------------------------------------
// Following
// -----------------------------------------------------------------------------

TEST_F(BusinessDayConventionTest, Following_WeekendMovesForward) {
    Date d(6, 4, 2024); // Samedi

    Date adjusted = adjustDate(d, calendar, BusinessDayConvention::Following);

    EXPECT_EQ(adjusted, Date(8, 4, 2024)); // Lundi
}

// -----------------------------------------------------------------------------
// Modified Following
// -----------------------------------------------------------------------------

TEST_F(BusinessDayConventionTest, ModifiedFollowing_MidMonthWeekendMovesForward) {
    Date d(6, 4, 2024); // Samedi 6 avril

    Date adjusted = adjustDate(d, calendar, BusinessDayConvention::ModifiedFollowing);

    EXPECT_EQ(adjusted, Date(8, 4, 2024)); // Lundi 8 avril
}

TEST_F(BusinessDayConventionTest, ModifiedFollowing_EndOfMonthMovesBackward) {
    Date d(31, 8, 2024); // Samedi 31 août

    Date adjusted = adjustDate(d, calendar, BusinessDayConvention::ModifiedFollowing);

    EXPECT_EQ(adjusted, Date(30, 8, 2024)); // Vendredi 30 août
}

TEST_F(BusinessDayConventionTest, ModifiedFollowing_HolidayMovesForward) {
    Date d(1, 5, 2024); // Férié (mercredi)

    Date adjusted = adjustDate(d, calendar, BusinessDayConvention::ModifiedFollowing);

    EXPECT_EQ(adjusted, Date(2, 5, 2024)); // Jeudi
}

// -----------------------------------------------------------------------------
// Preceding
// -----------------------------------------------------------------------------

TEST_F(BusinessDayConventionTest, Preceding_WeekendMovesBackward) {
    Date d(7, 4, 2024); // Dimanche

    Date adjusted = adjustDate(d, calendar, BusinessDayConvention::Preceding);

    EXPECT_EQ(adjusted, Date(5, 4, 2024)); // Vendredi
}

// -----------------------------------------------------------------------------
// Modified Preceding
// -----------------------------------------------------------------------------

TEST_F(BusinessDayConventionTest, ModifiedPreceding_EndOfMonthMovesForward) {
    Date d(1, 9, 2024); // Dimanche 1er septembre

    Date adjusted = adjustDate(d, calendar, BusinessDayConvention::ModifiedPreceding);

    EXPECT_EQ(adjusted, Date(2, 9, 2024)); // Lundi 2 septembre
}

TEST_F(BusinessDayConventionTest, ModifiedPreceding_MidMonthMovesBackward) {
    Date d(14, 7, 2024); // Dimanche 14 juillet

    Date adjusted = adjustDate(d, calendar, BusinessDayConvention::ModifiedPreceding);

    EXPECT_EQ(adjusted, Date(12, 7, 2024)); // Vendredi 12 juillet
}
