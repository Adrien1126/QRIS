#include "core/types/currency.hpp"
#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace qris::core {

namespace {
    void validateIsoCode(const std::string& code) {
        if (code.empty()) {
            throw std::invalid_argument("Currency ISO code cannot be empty");
        }

        if (code.size() != 3) {
            throw std::invalid_argument("Currency ISO code must have length 3");
        }

        if (!std::all_of(code.begin(), code.end(),
                         [](unsigned char c) { return std::isalpha(c); })) {
            throw std::invalid_argument("Currency ISO code must contain only letters");
        }
    }

    std::string normalizeIsoCode(std::string code) {
        std::transform(code.begin(), code.end(), code.begin(),
                       [](unsigned char c) { return std::toupper(c); });
        return code;
    }
} // namespace

Currency::Currency(std::string isoCode)
    : isoCode_(normalizeIsoCode(std::move(isoCode)))
{
    validateIsoCode(isoCode_);
}

const std::string& Currency::isoCode() const noexcept {
    return isoCode_;
}

bool Currency::operator==(const Currency& other) const noexcept {
    return isoCode_ == other.isoCode_;
}

bool Currency::operator!=(const Currency& other) const noexcept {
    return !(*this == other);
}

std::string Currency::toString() const {
    return isoCode_;
}

} // namespace qris::core
