#include "core/types/rateIndex.hpp"

#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace qris::core {

namespace {
void validateCode(const std::string& code)
{
    if (code.empty()) {
        throw std::invalid_argument("RateIndex code cannot be empty");
    }

    // Format attendu : NAME_TENOR (ex: EURIBOR_3M, ESTR_OIS)
    // Validation volontairement légère (structurelle, pas métier)
    if (code.find('_') == std::string::npos) {
        throw std::invalid_argument(
            "RateIndex code must follow NAME_TENOR format (e.g. EURIBOR_3M)");
    }
}

std::string normalizeCode(std::string code)
{
    std::transform(code.begin(), code.end(), code.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return code;
}

void validateFixingLag(int fixingLagBusinessDays)
{
    if (fixingLagBusinessDays < 0) {
        throw std::invalid_argument(
            "RateIndex fixing lag must be non-negative");
    }
}
} // namespace

RateIndex::RateIndex(std::string code,
                     Currency currency,
                     qris::time::Period tenor,
                     int fixingLagBusinessDays,
                     qris::time::DayCountConvention dayCount,
                     qris::time::Calendar fixingCalendar)
    : code_(normalizeCode(std::move(code))),
      currency_(std::move(currency)),
      tenor_(std::move(tenor)),
      fixingLagBusinessDays_(fixingLagBusinessDays),
      dayCount_(dayCount),
      fixingCalendar_(std::move(fixingCalendar))
{
    // Invariants structurels uniquement
    validateCode(code_);
    validateFixingLag(fixingLagBusinessDays_);
}

} // namespace qris::core
