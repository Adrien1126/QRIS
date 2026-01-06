#include "time/period.hpp"
#include <stdexcept>

namespace qris::time {
Period::Period(int multiplier, PeriodUnit unit) 
    : multiplier_(multiplier), unit_(unit)
{
    if (multiplier <= 0) {
        throw std::invalid_argument("Period: multiplier must be strictly positive"); 
    }
}

bool Period::operator==(const Period& other) const noexcept
{
    if (unit_ == other.unit_) {
        return multiplier_ == other.multiplier_;
    }

    if (unit_ == PeriodUnit::M && other.unit_ == PeriodUnit::Y) {
        return multiplier_ == 12 * other.multiplier_;
    }

    if (unit_ == PeriodUnit::Y && other.unit_ == PeriodUnit::M) {
        return 12 * multiplier_ == other.multiplier_;
    }

    return false;
}

bool Period::operator<(const Period& other) const
{
    if (unit_ == other.unit_) {
        return multiplier_ < other.multiplier_;
    }
    if (unit_ == PeriodUnit::M && other.unit_ == PeriodUnit::Y) {
        return multiplier_ < 12 * other.multiplier_;
    }

    if (unit_ == PeriodUnit::Y && other.unit_ == PeriodUnit::M) {
        return 12 * multiplier_ < other.multiplier_;
    }
    throw std::logic_error(
        "Period comparison error: periods are contractually incomparable");
}

bool Period::operator>(const Period& other) const
{
    if (*this == other) {
        return false;
    }

    return other < *this;
}
}