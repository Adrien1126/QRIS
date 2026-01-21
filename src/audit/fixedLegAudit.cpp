#include "audit/fixedLegAudit.hpp"
#include "audit/toString.hpp"

#include <iomanip>
#include <ostream>
#include <string>

namespace qris::audit {

using qris::contract::FixedLeg;
using qris::contract::Cashflow;

static const char* yesNo(bool v)
{
    return v ? "YES" : "NO";
}

void dumpFixedLeg(
    const FixedLeg& leg,
    std::ostream& out
)
{
    const auto& cfs = leg.cashflows();

    // ============================================================
    // HEADER
    // ============================================================
    out << "=========================================\n";
    out << " FIXED LEG — CONTRACTUAL AUDIT REPORT\n";
    out << "=========================================\n\n";

    // ============================================================
    // CONTRACTUAL PARAMETERS
    // ============================================================
    out << "Contractual parameters\n";
    out << "----------------------\n";
    out << "Start date              : " << leg.startDate().toString() << "\n";
    out << "End date                : " << leg.endDate().toString() << "\n";
    out << "Accrual period          : " << toString(leg.accrualPeriod()) << "\n";
    out << "Stub rule               : " << toString(leg.stubRule()) << "\n";
    out << "Business day convention : " << toString(leg.businessDayConvention()) << "\n";
    out << "Payment lag (days)      : " << leg.paymentDelay() << "\n";
    out << "Calendar                : " << leg.calendar().name() << "\n";
    out << "Notional                : " << leg.notional() << "\n";
    out << "Currency                : " << leg.currency().isoCode() << "\n";
    out << "Pay / Receive           : " << toString(leg.payReceive()) << "\n\n";

    // ============================================================
    // CASHFLOW SCHEDULE
    // ============================================================
    out << std::left
        << std::setw(4)  << "#"
        << std::setw(14) << "AccrualStart"
        << std::setw(14) << "AccrualEnd"
        << std::setw(8)  << "Days"
        << std::setw(12) << "YearFrac"
        << std::setw(14) << "PaymentDate"
        << "\n";

    out << std::string(64, '-') << "\n";

    std::size_t idx = 1;
    for (const Cashflow& cf : cfs) {

        const auto& as = *cf.accrualStartDate();
        const auto& ae = *cf.accrualEndDate();

        int days = ae.diff(as);

        auto dcc = cf.coupon().dayCount();
        double yf = qris::time::yearFraction(as, ae, dcc);

        out << std::left
            << std::setw(4)  << idx++
            << std::setw(14) << as.toString()
            << std::setw(14) << ae.toString()
            << std::setw(8)  << days
            << std::setw(12) << std::fixed << std::setprecision(6) << yf
            << std::setw(14) << cf.paymentDate().toString()
            << "\n";
    }

    // ============================================================
    // STRUCTURAL CHECKS
    // ============================================================
    out << "\nStructural checks\n";
    out << "-----------------\n";

    bool contiguous = true;
    for (std::size_t i = 1; i < cfs.size(); ++i) {
        if (*cfs[i - 1].accrualEndDate() !=
            *cfs[i].accrualStartDate()) {
            contiguous = false;
            break;
        }
    }

    bool strictlyIncreasing = true;
    for (std::size_t i = 1; i < cfs.size(); ++i) {
        if (!(cfs[i - 1].paymentDate() <
              cfs[i].paymentDate())) {
            strictlyIncreasing = false;
            break;
        }
    }

    const auto* refCoupon = &cfs.front().coupon();
    bool sharedCoupon = true;
    for (const auto& cf : cfs) {
        if (&cf.coupon() != refCoupon) {
            sharedCoupon = false;
            break;
        }
    }

    out << "Accrual periods contiguous        : " << yesNo(contiguous) << "\n";
    out << "Payment dates strictly increasing : " << yesNo(strictlyIncreasing) << "\n";
    out << "Coupon shared across cashflows    : " << yesNo(sharedCoupon) << "\n";

    // ============================================================
    // CONCLUSION
    // ============================================================
    out << "\nAudit conclusion\n";
    out << "----------------\n";

    if (contiguous && strictlyIncreasing && sharedCoupon) {
        out << "STATUS : PASS — leg is contractually coherent\n";
    } else {
        out << "STATUS : FAIL — see structural checks above\n";
    }

    out << "\n=========================================\n";
}

} // namespace qris::audit
