#include "time/time_arithmetic.hpp"

#include <algorithm>
#include <stdexcept>

namespace {

// -----------------------------------------------------------------------------
// Helpers calendaires locaux (arithmétique pure, pas métier)
// -----------------------------------------------------------------------------

bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int month, int year)
{
    switch (month) {
    case 1: case 3: case 5: case 7:
    case 8: case 10: case 12:
        return 31;
    case 4: case 6: case 9: case 11:
        return 30;
    case 2:
        return isLeapYear(year) ? 29 : 28;
    default:
        throw std::logic_error("daysInMonth: invalid month");
    }
}

} // anonymous namespace

namespace qris::time {

// -----------------------------------------------------------------------------
// addPeriod
// -----------------------------------------------------------------------------

Date addPeriod(const Date& d, const Period& p)
{
    switch (p.unit()) {

    case PeriodUnit::D: {
        const int sn = d.dateToSerialNumber();
        return Date(sn + p.multiplier());
    }

    case PeriodUnit::W: {
        const int sn = d.dateToSerialNumber();
        return Date(sn + 7 * p.multiplier());
    }

    case PeriodUnit::M: {
        const int day   = d.day();
        const int month = d.month();
        const int year  = d.year();

        const int totalMonths = (month - 1) + p.multiplier();
        const int newYear  = year + totalMonths / 12;
        const int newMonth = (totalMonths % 12) + 1;

        const int lastDay = daysInMonth(newMonth, newYear);
        const int newDay  = std::min(day, lastDay);

        return Date(newDay, newMonth, newYear);
    }

    case PeriodUnit::Y: {
        const int day   = d.day();
        const int month = d.month();
        const int year  = d.year() + p.multiplier();

        if (month == 2 && day == 29 && !isLeapYear(year)) {
            return Date(28, 2, year);
        }

        return Date(day, month, year);
    }

    default:
        throw std::logic_error("addPeriod: unsupported PeriodUnit");
    }
}

// -----------------------------------------------------------------------------
// subtractPeriod
// -----------------------------------------------------------------------------

Date subtractPeriod(const Date& d, const Period& p)
{
    switch (p.unit()) {

    case PeriodUnit::D: {
        const int sn = d.dateToSerialNumber();
        return Date(sn - p.multiplier());
    }

    case PeriodUnit::W: {
        const int sn = d.dateToSerialNumber();
        return Date(sn - 7 * p.multiplier());
    }

    case PeriodUnit::M: {
        const int day   = d.day();
        const int month = d.month();
        const int year  = d.year();

        const int totalMonths = (month - 1) - p.multiplier();
        const int newYear  = year + totalMonths / 12;
        const int newMonth = (totalMonths % 12 + 12) % 12 + 1;

        const int lastDay = daysInMonth(newMonth, newYear);
        const int newDay  = std::min(day, lastDay);

        return Date(newDay, newMonth, newYear);
    }

    case PeriodUnit::Y: {
        const int day   = d.day();
        const int month = d.month();
        const int year  = d.year() - p.multiplier();

        if (month == 2 && day == 29 && !isLeapYear(year)) {
            return Date(28, 2, year);
        }

        return Date(day, month, year);
    }

    default:
        throw std::logic_error("subtractPeriod: unsupported PeriodUnit");
    }
}

} // namespace qris::time
