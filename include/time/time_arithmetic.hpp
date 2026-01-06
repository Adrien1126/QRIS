#pragma once
#include "time/date.hpp"
#include "time/period.hpp"

namespace qris::time {
Date addPeriod(const Date& d, const Period& p); 
Date subtractPeriod(const Date& d, const Period& p); 
}