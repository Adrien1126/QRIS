#pragma once

#include <string>
#include "time/period.hpp"
#include "time/periodUnit.hpp"
#include "time/stubRule.hpp"
#include "time/businessDayConvention.hpp"
#include "core/types/payReceive.hpp"

namespace qris::audit {

std::string toString(const qris::time::Period& p);
std::string toString(qris::time::PeriodUnit u);
std::string toString(qris::time::StubRule r);
std::string toString(qris::time::BusinessDayConvention bdc);
std::string toString(qris::core::PayReceive pr);

}
