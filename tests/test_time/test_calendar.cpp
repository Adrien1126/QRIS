#include <gtest/gtest.h>

#include "time/date.hpp"
#include "time/calendar.hpp"

using namespace qris::time;

// -----------------------------------------------------------------------------
// FIXTURE DE TEST
// -----------------------------------------------------------------------------

class CalendarTest : public ::testing::Test {
protected:
    Calendar calendar;

    CalendarTest()
        : calendar(
              "TARGET",
              {
                  // Jours fériés explicites (exemples France / zone euro)
                  Date(1, 5, 2024),   // Fête du travail
                  Date(8, 5, 2024),   // Victoire 1945
                  Date(25, 12, 2024)  // Noël
              })
    {}
};

// -----------------------------------------------------------------------------
// TESTS : JOURS OUVRÉS STANDARD
// -----------------------------------------------------------------------------

TEST_F(CalendarTest, WeekdayIsBusinessDay)
{
    Date d(6, 5, 2024); // Lundi 6 mai 2024
    EXPECT_TRUE(calendar.isBusinessDay(d));
}

// -----------------------------------------------------------------------------
// TESTS : WEEK-END
// -----------------------------------------------------------------------------

TEST_F(CalendarTest, SaturdayIsNotBusinessDay)
{
    Date d(4, 5, 2024); // Samedi
    EXPECT_FALSE(calendar.isBusinessDay(d));
}

TEST_F(CalendarTest, SundayIsNotBusinessDay)
{
    Date d(5, 5, 2024); // Dimanche
    EXPECT_FALSE(calendar.isBusinessDay(d));
}

// -----------------------------------------------------------------------------
// TESTS : JOURS FÉRIÉS
// -----------------------------------------------------------------------------

TEST_F(CalendarTest, HolidayOnWeekdayIsNotBusinessDay)
{
    Date d(8, 5, 2024); // Mercredi férié
    EXPECT_FALSE(calendar.isBusinessDay(d));
}

TEST_F(CalendarTest, HolidayOnWeekendIsStillNotBusinessDay)
{
    Date d(25, 12, 2021); // Noël 2021 (samedi)
    EXPECT_FALSE(calendar.isBusinessDay(d));
}

// -----------------------------------------------------------------------------
// TESTS : IDENTITÉ DU CALENDRIER
// -----------------------------------------------------------------------------

TEST_F(CalendarTest, CalendarNameIsExposed)
{
    EXPECT_EQ(calendar.name(), "TARGET");
}
