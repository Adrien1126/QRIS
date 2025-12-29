#pragma once
#include "time/date.hpp"
#include "time/dayCountConventionType.hpp"

namespace qris::time {
double yearFraction(const Date& start, const Date& end, DayCountConvention convention); 
}