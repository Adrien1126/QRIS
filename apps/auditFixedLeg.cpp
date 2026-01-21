#include <iostream>

#include "audit/fixedLegAudit.hpp"

#include "contract/leg/fixedLeg.hpp"
#include "time/date.hpp"
#include "time/period.hpp"
#include "time/calendar.hpp"
#include "time/businessDayConvention.hpp"
#include "time/stubRule.hpp"

using namespace qris::contract;
using namespace qris::time;

int main()
{
    // ------------------------------------------------------------
    // TARGET calendar (simplified but realistic)
    // ------------------------------------------------------------
    Calendar target(
        "TARGET",
        {
            Date(1, 1, 2025),   // New Year
            Date(18, 4, 2025),  // Good Friday
            Date(21, 4, 2025),  // Easter Monday
            Date(1, 5, 2025),   // Labour Day
            Date(25, 12, 2025),  // Christmas
            Date(31, 12, 2025), // Saint Sylvestre
        }
    );

    // ------------------------------------------------------------
    // Fixed leg with FOLLOWING
    // ------------------------------------------------------------
    FixedLeg legFollowing(
        Date(30, 11, 2025),
        Date(31, 1, 2026),
        Period(1, PeriodUnit::M),
        target,
        BusinessDayConvention::Following,
        StubRule::ShortBack,
        0,                       // IMPORTANT: no payment lag
        1'000'000.0,
        qris::core::Currency("EUR"),
        qris::core::PayReceive::Receive,
        0.02,
        DayCountConvention::ACT_365F
    );

    // ------------------------------------------------------------
    // Fixed leg with MODIFIED FOLLOWING
    // ------------------------------------------------------------
    FixedLeg legModifiedFollowing(
        Date(30, 11, 2025),
        Date(31, 1, 2026),
        Period(1, PeriodUnit::M),
        target,
        BusinessDayConvention::ModifiedFollowing,
        StubRule::ShortBack,
        0,                       // IMPORTANT: no payment lag
        1'000'000.0,
        qris::core::Currency("EUR"),
        qris::core::PayReceive::Receive,
        0.02,
        DayCountConvention::ACT_360
    );

    // ------------------------------------------------------------
    // Audit outputs
    // ------------------------------------------------------------
    std::cout << "\n=========================================\n";
    std::cout << " FIXED LEG – FOLLOWING, ACT_365F\n";
    std::cout << "=========================================\n\n";

    qris::audit::dumpFixedLeg(legFollowing, std::cout);

    std::cout << "\n\n=========================================\n";
    std::cout << " FIXED LEG – MODIFIED FOLLOWING, ACT_360\n";
    std::cout << "=========================================\n\n";

    qris::audit::dumpFixedLeg(legModifiedFollowing, std::cout);

    return 0;
}
