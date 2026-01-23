#include <iostream>

#include "audit/floatingLegAudit.hpp"

#include "contract/leg/floatingLeg.hpp"
#include "core/types/rateIndex.hpp"
#include "core/types/currency.hpp"
#include "core/types/payReceive.hpp"

#include "time/date.hpp"
#include "time/period.hpp"
#include "time/calendar.hpp"
#include "time/businessDayConvention.hpp"
#include "time/stubRule.hpp"
#include "time/dayCountConvention.hpp"

using namespace qris::contract;
using namespace qris::time;
using namespace qris::core;

int main()
{
    // ------------------------------------------------------------
    // Calendrier TARGET avec un jour férié stratégique
    // ------------------------------------------------------------
    Calendar target(
        "TARGET",
        {
            Date(1, 1, 2025),
            Date(18, 4, 2025),
            Date(21, 4, 2025),
            Date(1, 5, 2025),
            Date(25, 12, 2025),
            Date(28, 11, 2025) // <-- volontairement proche du fixing
        }
    );

    // ------------------------------------------------------------
    // EURIBOR 3M
    // ------------------------------------------------------------
    auto euribor3m = std::make_shared<RateIndex>(
        "EURIBOR_3M",
        Currency("EUR"),
        Period(3, PeriodUnit::M),
        2,
        DayCountConvention::ACT_360,
        target
    );

    // ============================================================
    // CAS A — FloatingLeg standard (IBOR pédagogique)
    // ============================================================
    FloatingLeg standardLeg(
        Date(30, 11, 2025),
        Date(31, 1, 2026),
        Period(1, PeriodUnit::M),
        target,
        BusinessDayConvention::Following,
        StubRule::ShortBack,
        0,          // payment lag
        2,          // fixing lag
        1'000'000.0,
        Currency("EUR"),
        PayReceive::Receive,
        euribor3m,
        0.0015,
        DayCountConvention::ACT_360
    );

    std::cout << "\n=========================================\n";
    std::cout << " FLOATING LEG — STANDARD EURIBOR 3M\n";
    std::cout << "=========================================\n\n";

    qris::audit::dumpFloatingLeg(standardLeg, std::cout);

    // ============================================================
    // CAS B — FloatingLeg avec friction calendrier (audit réel)
    // ============================================================
    FloatingLeg holidayStressLeg(
        Date(1, 12, 2025),
        Date(31, 1, 2026),
        Period(1, PeriodUnit::M),
        target,
        BusinessDayConvention::Following,
        StubRule::ShortBack,
        0,
        2,
        1'000'000.0,
        Currency("EUR"),
        PayReceive::Receive,
        euribor3m,
        0.0015,
        DayCountConvention::ACT_360
    );

    std::cout << "\n\n=========================================\n";
    std::cout << " FLOATING LEG — FIXING CALENDAR STRESS\n";
    std::cout << "=========================================\n\n";

    qris::audit::dumpFloatingLeg(holidayStressLeg, std::cout);

    return 0;
}
