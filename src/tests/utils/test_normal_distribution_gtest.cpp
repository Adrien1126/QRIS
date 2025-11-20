#include <gtest/gtest.h>
#include "core/utils/normal_distribution.hpp"

using qris::math::NormalDistribution;

TEST(NormalDistributionGTest, PdfAtZero) {
    constexpr double expected = 0.39894228040143267794;
    EXPECT_NEAR(NormalDistribution::pdf(0.0), expected, 1e-14);
}

TEST(NormalDistributionGTest, PdfSymmetry) {
    EXPECT_NEAR(NormalDistribution::pdf(1.0),
                NormalDistribution::pdf(-1.0),
                1e-14);
}

TEST(NormalDistributionGTest, CdfAtZero) {
    EXPECT_NEAR(NormalDistribution::cdf(0.0), 0.5, 1e-15);
}

TEST(NormalDistributionGTest, CdfAtOne) {
    EXPECT_NEAR(NormalDistribution::cdf(1.0),
                0.8413447460685429,
                1e-12);
}
