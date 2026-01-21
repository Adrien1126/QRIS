#include "audit/toString.hpp"

namespace qris::audit {

std::string toString(const qris::time::Period& p)
{
    return std::to_string(p.multiplier()) + toString(p.unit());
}

std::string toString(qris::time::PeriodUnit u)
{
    switch (u) {
        case qris::time::PeriodUnit::D: return "D";
        case qris::time::PeriodUnit::W: return "W";
        case qris::time::PeriodUnit::M: return "M";
        case qris::time::PeriodUnit::Y: return "Y";
    }
    return "?";
}

std::string toString(qris::time::StubRule r)
{
    switch (r) {
        case qris::time::StubRule::ShortFront: return "ShortFront";
        case qris::time::StubRule::LongFront:  return "LongFront";
        case qris::time::StubRule::ShortBack:  return "ShortBack";
        case qris::time::StubRule::LongBack:   return "LongBack";
    }
    return "?";
}

std::string toString(qris::time::BusinessDayConvention bdc)
{
    switch (bdc) {
        case qris::time::BusinessDayConvention::Unadjusted:        return "Unadjusted";
        case qris::time::BusinessDayConvention::Following:         return "Following";
        case qris::time::BusinessDayConvention::ModifiedFollowing: return "ModifiedFollowing";
        case qris::time::BusinessDayConvention::Preceding:         return "Preceding";
        case qris::time::BusinessDayConvention::ModifiedPreceding: return "ModifiedPreceding";
    }
    return "?";
}

std::string toString(qris::core::PayReceive pr)
{
    return (pr == qris::core::PayReceive::Pay) ? "Pay" : "Receive";
}

}
