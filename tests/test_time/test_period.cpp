#include <gtest/gtest.h>
#include <stdexcept>

#include "time/period.hpp"
#include "time/periodUnit.hpp"

using qris::time::Period;
using qris::time::PeriodUnit;

// -----------------------------------------------------------------------------
// Construction & invariants
// -----------------------------------------------------------------------------

TEST(PeriodConstructionTest, ValidConstruction)
{
    Period p1(1, PeriodUnit::D);
    Period p2(3, PeriodUnit::M);
    Period p3(2, PeriodUnit::Y);

    EXPECT_EQ(p1.multiplier(), 1);
    EXPECT_EQ(p1.unit(), PeriodUnit::D);

    EXPECT_EQ(p2.multiplier(), 3);
    EXPECT_EQ(p2.unit(), PeriodUnit::M);

    EXPECT_EQ(p3.multiplier(), 2);
    EXPECT_EQ(p3.unit(), PeriodUnit::Y);
}

TEST(PeriodConstructionTest, ZeroMultiplierThrows)
{
    EXPECT_THROW(
        Period(0, PeriodUnit::M),
        std::invalid_argument);
}

TEST(PeriodConstructionTest, NegativeMultiplierThrows)
{
    EXPECT_THROW(
        Period(-3, PeriodUnit::Y),
        std::invalid_argument);
}

// -----------------------------------------------------------------------------
// Equality (contractual equivalence)
// -----------------------------------------------------------------------------

TEST(PeriodEqualityTest, SameUnitSameMultiplier)
{
    EXPECT_TRUE(Period(3, PeriodUnit::M) == Period(3, PeriodUnit::M));
    EXPECT_FALSE(Period(3, PeriodUnit::M) == Period(6, PeriodUnit::M));
}

TEST(PeriodEqualityTest, MonthYearEquivalence)
{
    EXPECT_TRUE(Period(12, PeriodUnit::M) == Period(1, PeriodUnit::Y));
    EXPECT_TRUE(Period(24, PeriodUnit::M) == Period(2, PeriodUnit::Y));

    EXPECT_FALSE(Period(6, PeriodUnit::M) == Period(1, PeriodUnit::Y));
}

TEST(PeriodEqualityTest, NoOtherEquivalenceAllowed)
{
    EXPECT_FALSE(Period(4, PeriodUnit::W) == Period(1, PeriodUnit::M));
    EXPECT_FALSE(Period(30, PeriodUnit::D) == Period(1, PeriodUnit::M));
    EXPECT_FALSE(Period(365, PeriodUnit::D) == Period(1, PeriodUnit::Y));
}

// -----------------------------------------------------------------------------
// Ordering (partial order)
// -----------------------------------------------------------------------------

TEST(PeriodOrderingTest, SameUnitOrdering)
{
    EXPECT_TRUE(Period(3, PeriodUnit::M) < Period(6, PeriodUnit::M));
    EXPECT_TRUE(Period(2, PeriodUnit::Y) > Period(1, PeriodUnit::Y));

    EXPECT_FALSE(Period(6, PeriodUnit::M) < Period(3, PeriodUnit::M));
}

TEST(PeriodOrderingTest, MonthYearOrdering)
{
    EXPECT_TRUE(Period(6, PeriodUnit::M) < Period(1, PeriodUnit::Y));
    EXPECT_TRUE(Period(18, PeriodUnit::M) > Period(1, PeriodUnit::Y));

    EXPECT_FALSE(Period(12, PeriodUnit::M) < Period(1, PeriodUnit::Y));
    EXPECT_FALSE(Period(12, PeriodUnit::M) > Period(1, PeriodUnit::Y));
}

TEST(PeriodOrderingTest, EqualityDoesNotImplyOrdering)
{
    Period p1(12, PeriodUnit::M);
    Period p2(1, PeriodUnit::Y);

    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 < p2);
    EXPECT_FALSE(p1 > p2);
}

// -----------------------------------------------------------------------------
// Incomparability (must throw)
// -----------------------------------------------------------------------------

TEST(PeriodOrderingTest, IncomparablePeriodsThrow)
{
    EXPECT_THROW(
        {
            Period p1(15, PeriodUnit::D);
            Period p2(1, PeriodUnit::M);
            (void)(p1 < p2);
        },
        std::logic_error);

    EXPECT_THROW(
        {
            Period p1(1, PeriodUnit::W);
            Period p2(1, PeriodUnit::M);
            (void)(p1 > p2);
        },
        std::logic_error);

    EXPECT_THROW(
        {
            Period p1(7, PeriodUnit::D);
            Period p2(1, PeriodUnit::W);
            (void)(p1 < p2);
        },
        std::logic_error);
}


// -----------------------------------------------------------------------------
// Symmetry & consistency
// -----------------------------------------------------------------------------

TEST(PeriodConsistencyTest, OrderingIsAntiSymmetric)
{
    Period shortP(3, PeriodUnit::M);
    Period longP(6, PeriodUnit::M);

    EXPECT_TRUE(shortP < longP);
    EXPECT_TRUE(longP > shortP);
    EXPECT_FALSE(longP < shortP);
}

TEST(PeriodConsistencyTest, EqualityIsSymmetric)
{
    Period p1(12, PeriodUnit::M);
    Period p2(1, PeriodUnit::Y);

    EXPECT_TRUE(p1 == p2);
    EXPECT_TRUE(p2 == p1);
}
