#include "time/calendar.hpp"
#include "time/weekDay.hpp"

namespace qris::time {
    bool Calendar::isBusinessDay(const Date& date) const
    {
        WeekDay wd = date.weekDay();

        if (wd == WeekDay::Saturday || wd == WeekDay::Sunday) {
            return false;
        }

        for (const Date& holiday : holidays_) {
            if (date == holiday) {
                return false;
            }
        }
        return true;
    }

}