#pragma once

#include <string>
#include "core/types/currency.hpp"
#include "time/period.hpp"
#include "time/dayCountConvention.hpp"
#include "time/calendar.hpp"

namespace qris::core {

class RateIndex final {
public:
    RateIndex(std::string code,
              Currency currency,
              qris::time::Period tenor,
              int fixingLagBusinessDays,
              qris::time::DayCountConvention dayCount,
              qris::time::Calendar fixingCalendar);

    const std::string& code() const noexcept {return code_;}
    const Currency& currency() const noexcept {return currency_;}
    const qris::time::Period& tenor() const noexcept {return tenor_;}
    int fixingLagBusinessDays() const noexcept {return fixingLagBusinessDays_;}
    qris::time::DayCountConvention dayCount() const noexcept {return dayCount_;}
    const qris::time::Calendar& fixingCalendar() const noexcept {return fixingCalendar_;}

private:
    const std::string code_;
    const Currency currency_;
    const qris::time::Period tenor_;
    const int fixingLagBusinessDays_;
    const qris::time::DayCountConvention dayCount_;
    const qris::time::Calendar fixingCalendar_;
};

} // namespace qris::core
