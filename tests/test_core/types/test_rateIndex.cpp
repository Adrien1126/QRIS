#include <gtest/gtest.h>

#include "core/types/rateIndex.hpp"
#include "core/types/currency.hpp"
#include "time/period.hpp"
#include "time/periodUnit.hpp"
#include "time/dayCountConvention.hpp"
#include "time/calendar.hpp"

using namespace qris::core;
using namespace qris::time;


static RateIndex makeEuribor3M()
{
    return RateIndex(
        "EURIBOR_3M",
        Currency("EUR"),
        Period(3, PeriodUnit::M),
        2,                              // fixing lag standard
        DayCountConvention::ACT_360,
        Calendar("TARGET", {})
    );
}

TEST(RateIndexTest, ValidConstruction)
{
    RateIndex index = makeEuribor3M();

    EXPECT_EQ(index.code(), "EURIBOR_3M");
    EXPECT_EQ(index.currency(), Currency("EUR"));
    EXPECT_EQ(index.tenor(), Period(3, PeriodUnit::M));
    EXPECT_EQ(index.fixingLagBusinessDays(), 2);
    EXPECT_EQ(index.dayCount(), DayCountConvention::ACT_360);
}

TEST(RateIndexTest, CodeIsNormalizedToUppercase)
{
    RateIndex index(
        "euribor_3m",
        Currency("eur"),
        Period(3, PeriodUnit::M),
        2,
        DayCountConvention::ACT_360,
        Calendar("TARGET", {})
    );

    EXPECT_EQ(index.code(), "EURIBOR_3M");
    EXPECT_EQ(index.currency(), Currency("EUR"));
}


TEST(RateIndexTest, EmptyCodeThrows)
{
    EXPECT_THROW(
        RateIndex(
            "",
            Currency("EUR"),
            Period(3, PeriodUnit::M),
            2,
            DayCountConvention::ACT_360,
            Calendar("TARGET", {})
        ),
        std::invalid_argument
    );
}

TEST(RateIndexTest, CodeWithoutUnderscoreThrows)
{
    EXPECT_THROW(
        RateIndex(
            "EURIBOR3M",
            Currency("EUR"),
            Period(3, PeriodUnit::M),
            2,
            DayCountConvention::ACT_360,
            Calendar("TARGET", {})
        ),
        std::invalid_argument
    );
}

TEST(RateIndexTest, NegativeFixingLagThrows)
{
    EXPECT_THROW(
        RateIndex(
            "EURIBOR_3M",
            Currency("EUR"),
            Period(3, PeriodUnit::M),
            -1,
            DayCountConvention::ACT_360,
            Calendar("TARGET", {})
        ),
        std::invalid_argument
    );
}

TEST(RateIndexTest, ImmutabilityAndStability)
{
    const RateIndex index = makeEuribor3M();

    // Les getters doivent être stables et cohérents
    EXPECT_EQ(index.code(), "EURIBOR_3M");
    EXPECT_EQ(index.currency(), Currency("EUR"));
    EXPECT_EQ(index.fixingLagBusinessDays(), 2);
}

TEST(RateIndexTest, DayCountIsDescriptiveOnly)
{
    RateIndex index(
        "EURIBOR_6M",
        Currency("EUR"),
        Period(6, PeriodUnit::M),
        2,
        DayCountConvention::ACT_365F,
        Calendar("TARGET", {})
    );

    EXPECT_EQ(index.dayCount(), DayCountConvention::ACT_365F);
}
