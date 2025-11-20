#include <gtest/gtest.h>
#include "core/models/BlackScholesModel.hpp"

using qris::model::BlackScholesModel;

TEST(BlackScholesModelTest, D1Value) {
    BlackScholesModel model(100.0, 0.05, 0.2);

    // Valeur de référence provenant de Python/Numpy :
    // d1 = 0.35000000000000003 (strike=100, T=1)
    double d1 = model.d1(100.0, 1.0);

    EXPECT_NEAR(d1, 0.35000000000000003, 1e-12);
}

TEST(BlackScholesModelTest, D2Value) {
    BlackScholesModel model(100.0, 0.05, 0.2);

    // Référence Python : d2 = 0.15000000000000002
    double d2 = model.d2(100.0, 1.0);

    EXPECT_NEAR(d2, 0.15000000000000002, 1e-12);
}

TEST(BlackScholesModelTest, ThrowsOnInvalidInputs) {
    BlackScholesModel model(100.0, 0.05, 0.2);

    EXPECT_THROW(model.d1(0.0, 1.0), std::invalid_argument);
    EXPECT_THROW(model.d1(100.0, 0.0), std::invalid_argument);
}
