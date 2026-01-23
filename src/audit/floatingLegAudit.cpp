#include "audit/floatingLegAudit.hpp"
#include "audit/toString.hpp"

#include "contract/coupon/floatingCoupon.hpp"

#include <iomanip>
#include <ostream>
#include <string>
#include <stdexcept>

namespace qris::audit {

using qris::contract::FloatingLeg;
using qris::contract::Cashflow;
using qris::contract::FloatingCoupon;

static const char* yesNo(bool v)
{
    return v ? "YES" : "NO";
}

void dumpFloatingLeg(
    const FloatingLeg& leg,
    std::ostream& out
)
{
    const auto& cfs = leg.cashflows();

    if (cfs.empty()) {
        out << "EMPTY FLOATING LEG — no cashflows generated\n";
        return;
    }

    // ------------------------------------------------------------
    // Coupon — single source of financial truth
    // ------------------------------------------------------------
    const auto* coupon =
        dynamic_cast<const FloatingCoupon*>(&cfs.front().coupon());

    if (!coupon) {
        throw std::logic_error(
            "FloatingLeg audit: coupon is not FloatingCoupon");
    }

    const auto& index = coupon->index();

    // ============================================================
    // HEADER
    // ============================================================
    out << "=========================================\n";
    out << " FLOATING LEG — CONTRACTUAL AUDIT REPORT\n";
    out << "=========================================\n\n";

    // ============================================================
    // CONTRACTUAL PARAMETERS (LEG LEVEL)
    // ============================================================
    out << "Contractual parameters\n";
    out << "----------------------\n";
    out << "Start date              : " << leg.startDate().toString() << "\n";
    out << "End date                : " << leg.endDate().toString() << "\n";
    out << "Accrual period          : " << toString(leg.accrualPeriod()) << "\n";
    out << "Stub rule               : " << toString(leg.stubRule()) << "\n";
    out << "Payment BDC             : " << toString(leg.businessDayConvention()) << "\n";
    out << "Payment lag (days)      : " << leg.paymentDelay() << "\n";
    out << "Calendar                : " << leg.calendar().name() << "\n";
    out << "Notional                : " << leg.notional() << "\n";
    out << "Currency                : " << leg.currency().isoCode() << "\n";
    out << "Pay / Receive           : " << toString(leg.payReceive()) << "\n\n";

    // ============================================================
    // FLOATING COUPON PARAMETERS (MARKET LEVEL)
    // ============================================================
    out << "Floating coupon parameters\n";
    out << "--------------------------\n";
    out << "Index code               : " << index.code() << "\n";
    out << "Index tenor              : " << toString(index.tenor()) << "\n";
    out << "Index fixing lag (bdays) : " << index.fixingLagBusinessDays() << "\n";
    out << "Index calendar           : " << index.fixingCalendar().name() << "\n";
    out << "Spread                   : " << coupon->spread() << "\n";
    out << "Coupon day count         : " << toString(coupon->dayCount()) << "\n\n";

    // ============================================================
    // CASHFLOW SCHEDULE (FIXING-CENTRIC)
    // ============================================================
    out << std::left
        << std::setw(4)  << "#"
        << std::setw(14) << "FixingDate"
        << std::setw(14) << "AccrualStart"
        << std::setw(14) << "AccrualEnd"
        << std::setw(8)  << "Days"
        << std::setw(12) << "YearFrac"
        << std::setw(14) << "PaymentDate"
        << "\n";

    out << std::string(76, '-') << "\n";

    std::size_t idx = 1;
    for (const Cashflow& cf : cfs) {

        const auto& fs = *cf.fixingDate();
        const auto& as = *cf.accrualStartDate();
        const auto& ae = *cf.accrualEndDate();

        int days = ae.diff(as);
        double yf = qris::time::yearFraction(
            as, ae, coupon->dayCount());

        out << std::left
            << std::setw(4)  << idx++
            << std::setw(14) << fs.toString()
            << std::setw(14) << as.toString()
            << std::setw(14) << ae.toString()
            << std::setw(8)  << days
            << std::setw(12) << std::fixed << std::setprecision(6) << yf
            << std::setw(14) << cf.paymentDate().toString()
            << "\n";
    }

    // ============================================================
    // STRUCTURAL & MARKET CHECKS (FLOATING-SPECIFIC)
    // ============================================================
    out << "\nStructural checks\n";
    out << "-----------------\n";

    bool contiguous        = true;
    bool increasing        = true;
    bool fixingBeforeStart = true;
    bool sharedCoupon      = true;
    bool sharedIndex       = true;

    const auto* refCoupon = &cfs.front().coupon();
    const auto* refIndex  = &index;

    for (std::size_t i = 1; i < cfs.size(); ++i) {

        if (*cfs[i - 1].accrualEndDate() !=
            *cfs[i].accrualStartDate()) {
            contiguous = false;
        }

        if (!(cfs[i - 1].paymentDate() <
              cfs[i].paymentDate())) {
            increasing = false;
        }

        if (!(*cfs[i].fixingDate() <
              *cfs[i].accrualStartDate())) {
            fixingBeforeStart = false;
        }

        if (&cfs[i].coupon() != refCoupon) {
            sharedCoupon = false;
        }

        const auto* cpn =
            dynamic_cast<const FloatingCoupon*>(&cfs[i].coupon());

        if (!cpn || &cpn->index() != refIndex) {
            sharedIndex = false;
        }
    }

    out << "Accrual periods contiguous             : " << yesNo(contiguous) << "\n";
    out << "Payment dates strictly increasing      : " << yesNo(increasing) << "\n";
    out << "Fixing strictly before accrual start   : " << yesNo(fixingBeforeStart) << "\n";
    out << "Coupon shared across cashflows         : " << yesNo(sharedCoupon) << "\n";
    out << "Index shared across cashflows          : " << yesNo(sharedIndex) << "\n";

    // ============================================================
    // CONCLUSION
    // ============================================================
    out << "\nAudit conclusion\n";
    out << "----------------\n";

    if (contiguous && increasing &&
        fixingBeforeStart && sharedCoupon && sharedIndex) {
        out << "STATUS : PASS — floating leg is contractually coherent\n";
    } else {
        out << "STATUS : FAIL — see checks above\n";
    }

    out << "\n=========================================\n";
}

} // namespace qris::audit
