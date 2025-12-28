#include "time/businessDayConvention.hpp"

namespace qris::time {

Date adjustDate(const Date& date,
                const Calendar& calendar,
                BusinessDayConvention convention)
{
    if (calendar.isBusinessDay(date)) {
        return date;
    }

    switch (convention) {

    case BusinessDayConvention::Unadjusted:
        return date;

    case BusinessDayConvention::Following: {
        Date current = date.addDays(1);
        while (!calendar.isBusinessDay(current)) {
            current = current.addDays(1);
        }
        return current;
    }

    case BusinessDayConvention::ModifiedFollowing: {
        Date forward = date.addDays(1);
        while (!calendar.isBusinessDay(forward)) {
            forward = forward.addDays(1);
        }

        // Vérification du changement de mois
        if (forward.month() != date.month()) {
            Date backward = date.addDays(-1);
            while (!calendar.isBusinessDay(backward)) {
                backward = backward.addDays(-1);
            }
            return backward;
        }

        return forward;
    }

    case BusinessDayConvention::Preceding: {
        Date current = date.addDays(-1);
        while (!calendar.isBusinessDay(current)) {
            current = current.addDays(-1);
        }
        return current;
    }

    case BusinessDayConvention::ModifiedPreceding: {
        Date backward = date.addDays(-1);
        while (!calendar.isBusinessDay(backward)) {
            backward = backward.addDays(-1);
        }

        if (backward.month() != date.month()) {
            Date forward = date.addDays(1);
            while (!calendar.isBusinessDay(forward)) {
                forward = forward.addDays(1);
            }
            return forward;
        }

        return backward;
    }
    }

    return date;
}

} // namespace qris::time
