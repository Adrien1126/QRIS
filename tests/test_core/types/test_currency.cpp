#include <gtest/gtest.h>
#include "core/types/currency.hpp"

using qris::core::Currency;

// -----------------------------------------------------------------------------
// Construction & normalisation
// -----------------------------------------------------------------------------

TEST(CurrencyTest, ConstructValidUppercase) {
    Currency eur("EUR");

    EXPECT_EQ(eur.isoCode(), "EUR");
}

TEST(CurrencyTest, ConstructValidLowercaseIsNormalized) {
    Currency usd("usd");

    EXPECT_EQ(usd.isoCode(), "USD");
}

TEST(CurrencyTest, ConstructMixedCaseIsNormalized) {
    Currency jpy("jPy");

    EXPECT_EQ(jpy.isoCode(), "JPY");
}

// -----------------------------------------------------------------------------
// Validation des erreurs
// -----------------------------------------------------------------------------

TEST(CurrencyTest, EmptyIsoCodeThrows) {
    EXPECT_THROW(Currency(""), std::invalid_argument);
}

TEST(CurrencyTest, IsoCodeTooShortThrows) {
    EXPECT_THROW(Currency("EU"), std::invalid_argument);
}

TEST(CurrencyTest, IsoCodeTooLongThrows) {
    EXPECT_THROW(Currency("EURO"), std::invalid_argument);
}

TEST(CurrencyTest, IsoCodeWithDigitsThrows) {
    EXPECT_THROW(Currency("E1R"), std::invalid_argument);
}

TEST(CurrencyTest, IsoCodeWithSymbolsThrows) {
    EXPECT_THROW(Currency("E$R"), std::invalid_argument);
}

// -----------------------------------------------------------------------------
// Comparaison
// -----------------------------------------------------------------------------

TEST(CurrencyTest, EqualitySameIso) {
    Currency eur1("EUR");
    Currency eur2("eur");

    EXPECT_TRUE(eur1 == eur2);
    EXPECT_FALSE(eur1 != eur2);
}

TEST(CurrencyTest, InequalityDifferentIso) {
    Currency eur("EUR");
    Currency usd("USD");

    EXPECT_TRUE(eur != usd);
    EXPECT_FALSE(eur == usd);
}

// -----------------------------------------------------------------------------
// Représentation textuelle
// -----------------------------------------------------------------------------

TEST(CurrencyTest, ToStringReturnsIsoCode) {
    Currency gbp("gbp");

    EXPECT_EQ(gbp.toString(), "GBP");
}
