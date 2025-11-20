#include <gtest/gtest.h>
#include "core/instruments/EuropeanOption.hpp"

using qris::instruments::EuropeanOption;
using qris::instruments::OptionType;

TEST(EuropeanOptionTest, ConstructorStoresValues) {
    EuropeanOption opt(100.0, 1.0, OptionType::Call);

    EXPECT_DOUBLE_EQ(opt.get_strike(), 100.0);
    EXPECT_DOUBLE_EQ(opt.get_maturity(), 1.0);
    EXPECT_TRUE(opt.isCall());
    EXPECT_FALSE(opt.isPut());
}

TEST(EuropeanOptionTest, PutOptionCheck) {
    EuropeanOption opt(50.0, 0.5, OptionType::Put);

    EXPECT_TRUE(opt.isPut());
    EXPECT_FALSE(opt.isCall());
}

TEST(EuropeanOptionTest, ThrowsOnInvalidInputs) {
    EXPECT_THROW(EuropeanOption(0.0, 1.0, OptionType::Call),
                 std::invalid_argument);

    EXPECT_THROW(EuropeanOption(100.0, 0.0, OptionType::Put),
                 std::invalid_argument);
}
