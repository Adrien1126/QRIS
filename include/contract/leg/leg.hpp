#pragma once

#include <vector>

#include "contract/cashflow.hpp"
#include "time/period.hpp"
#include "time/calendar.hpp"
#include "time/businessDayConvention.hpp"
#include "time/stubRule.hpp"

namespace qris::contract {

class Leg {
public:
    virtual ~Leg() = default;

    // ---------------------------------------------------------------------
    // Accès aux cashflows générés
    // ---------------------------------------------------------------------
    const std::vector<Cashflow>& cashflows() const noexcept {
        return cashflows_;
    }

    // ---------------------------------------------------------------------
    // Accesseurs contractuels (lecture seule)
    // ---------------------------------------------------------------------

    const qris::time::Date& startDate() const noexcept {
        return startDate_;
    }

    const qris::time::Date& endDate() const noexcept {
        return endDate_;
    }

    const qris::time::Period& accrualPeriod() const noexcept {
        return accrualPeriod_;
    }

    const qris::time::Calendar& calendar() const noexcept {
        return calendar_;
    }

    qris::time::BusinessDayConvention businessDayConvention() const noexcept {
        return bdc_;
    }

    qris::time::StubRule stubRule() const noexcept {
        return stubRule_;
    }

    int paymentDelay() const noexcept {
        return paymentDelay_;
    }

    double notional() const noexcept {
        return notional_;
    }

    const qris::core::Currency& currency() const noexcept {
        return currency_;
    }

    qris::core::PayReceive payReceive() const noexcept {
        return payReceive_;
    }

protected:
    // ---------------------------------------------------------------------
    // Constructeur protégé (classe abstraite)
    // ---------------------------------------------------------------------
    Leg(
        qris::time::Date startDate,
        qris::time::Date endDate,
        qris::time::Period accrualPeriod,
        qris::time::Calendar calendar,
        qris::time::BusinessDayConvention bdc,
        qris::time::StubRule stubRule,
        int paymentDelay,
        double notional,
        qris::core::Currency currency,
        qris::core::PayReceive payReceive
    );

    // ---------------------------------------------------------------------
    // Paramètres contractuels communs
    // ---------------------------------------------------------------------
    qris::time::Date startDate_;
    qris::time::Date endDate_;
    qris::time::Period accrualPeriod_;
    qris::time::Calendar calendar_;
    qris::time::BusinessDayConvention bdc_;
    qris::time::StubRule stubRule_;
    int paymentDelay_;

    double notional_;
    qris::core::Currency currency_;
    qris::core::PayReceive payReceive_;

    std::vector<Cashflow> cashflows_;

    // ---------------------------------------------------------------------
    // Invariants globaux de la leg
    // ---------------------------------------------------------------------
    void validateInvariants() const;
};

} // namespace qris::contract
