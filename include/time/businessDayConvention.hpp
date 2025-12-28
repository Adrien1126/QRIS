#pragma once
#include "time/date.hpp"
#include "time/calendar.hpp"
#include "time/businessDayConventionType.hpp"

namespace qris::time {

Date adjustDate(
    const Date& date,
    const Calendar& calendar,
    BusinessDayConvention convention
);

}
