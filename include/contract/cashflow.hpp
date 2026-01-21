#pragma once

#include <memory>
#include <optional>

#include "time/date.hpp"
#include "core/types/currency.hpp"
#include "core/types/payReceive.hpp"

namespace qris::contract {

using qris::time::Date;

// Forward declaration pour éviter dépendance forte
class Coupon;

class Cashflow {
private:
    // Informations temporelles
    Date paymentDate_;
    std::optional<Date> fixingDate_;
    std::optional<Date> accrualStartDate_;
    std::optional<Date> accrualEndDate_;

    // Informations financières
    qris::core::Currency currency_;
    double notional_;
    qris::core::PayReceive direction_;
    std::shared_ptr<const Coupon> coupon_;

private:
    // Validation des invariants contractuels
    void validateInvariants() const;

public:
    // Constructeur
    Cashflow(
        Date paymentDate,
        std::optional<Date> fixingDate,
        std::optional<Date> accrualStartDate,
        std::optional<Date> accrualEndDate,
        qris::core::Currency currency,
        double notional,
        qris::core::PayReceive direction,
        std::shared_ptr<const Coupon> coupon);

    // Getters temporels
    const Date& paymentDate() const { return paymentDate_; }

    const std::optional<Date>& fixingDate() const { return fixingDate_; }
    const std::optional<Date>& accrualStartDate() const { return accrualStartDate_; }
    const std::optional<Date>& accrualEndDate() const { return accrualEndDate_; }

    // Getters financiers
    const qris::core::Currency& currency() const { return currency_; }
    double notional() const { return notional_; }
    qris::core::PayReceive direction() const { return direction_; }

    const Coupon& coupon() const { return *coupon_; }
};

} // namespace qris::contract
