#include <iostream>
#include <memory>

#include "contract/cashflow.hpp"
#include "contract/coupon/fixedCoupon.hpp"
#include "contract/coupon/floatingCoupon.hpp"

using namespace qris::contract;
using qris::time::Date;
using qris::time::DayCountConvention;
using qris::core::Currency;
using qris::core::PayReceive;

static Currency EUR()
{
    return Currency("EUR");
}

int main()
{
    try {
        // ---------------------------------------------------------------------
        // Cashflow fixe
        // ---------------------------------------------------------------------
        auto fixedCoupon = std::make_unique<FixedCoupon>(
            0.025,                         // 2.5%
            DayCountConvention::ACT_360);

        Cashflow fixedCashflow(
            Date(30, 6, 2025),              // payment date
            std::nullopt,                   // no fixing
            Date(30, 3, 2025),              // accrual start
            Date(30, 6, 2025),              // accrual end
            EUR(),
            1'000'000.0,
            PayReceive::Receive,
            std::move(fixedCoupon));

        std::cout << "Fixed Cashflow\n";
        std::cout << "  Payment date  : " << fixedCashflow.paymentDate().toString() << "\n";
        std::cout << "  Currency      : " << fixedCashflow.currency().toString() << "\n";
        std::cout << "  Notional      : " << fixedCashflow.notional() << "\n";
        std::cout << "  Direction     : Receive\n";
        std::cout << "  Coupon type   : Fixed\n";
        std::cout << "  Day count     : ACT/360\n\n";

        // ---------------------------------------------------------------------
        // Cashflow flottant
        // ---------------------------------------------------------------------
        auto floatingCoupon = std::make_unique<FloatingCoupon>(
            "EURIBOR_3M",
            0.0010,                         // +10 bps
            DayCountConvention::ACT_360);

        Cashflow floatingCashflow(
            Date(30, 9, 2025),              // payment date
            Date(27, 6, 2025),              // fixing
            Date(30, 6, 2025),              // accrual start
            Date(30, 9, 2025),              // accrual end
            EUR(),
            5'000'000.0,
            PayReceive::Pay,
            std::move(floatingCoupon));

        std::cout << "Floating Cashflow\n";
        std::cout << "  Payment date  : " << floatingCashflow.paymentDate().toString() << "\n";
        std::cout << "  Fixing date   : " << floatingCashflow.fixingDate()->toString() << "\n";
        std::cout << "  Currency      : " << floatingCashflow.currency().toString() << "\n";
        std::cout << "  Notional      : " << floatingCashflow.notional() << "\n";
        std::cout << "  Direction     : Pay\n";
        std::cout << "  Coupon type   : Floating\n";
        std::cout << "  Index         : EURIBOR_3M\n";
        std::cout << "  Day count     : ACT/360\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "Error while constructing cashflows: "
                  << ex.what() << '\n';
        return 1;
    }

    return 0;
}
