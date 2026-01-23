#include "contract/leg/fixedLeg.hpp"

#include "time/scheduleBuilder.hpp"
#include "time/businessDayConvention.hpp"
#include "time/time_arithmetic.hpp"

#include <algorithm>
#include <stdexcept>

namespace qris::contract {

using qris::time::ScheduleBuilder;
using qris::time::adjustDate;

FixedLeg::FixedLeg(
    qris::time::Date startDate,
    qris::time::Date endDate,
    qris::time::Period accrualPeriod,
    qris::time::Calendar calendar,
    qris::time::BusinessDayConvention bdc,
    qris::time::StubRule stubRule,
    int paymentDelay,
    double notional,
    qris::core::Currency currency,
    qris::core::PayReceive payReceive,
    double fixedRate,
    qris::time::DayCountConvention dayCount
)
: Leg(
      startDate,
      endDate,
      accrualPeriod,
      calendar,
      bdc,
      stubRule,
      paymentDelay,
      notional,
      currency,
      payReceive
  )
, coupon_(std::make_shared<FixedCoupon>(fixedRate, dayCount))
{
    if (fixedRate <= 0.0) {
        throw std::invalid_argument(
            "FixedLeg: fixedRate must be strictly positive");
    }

    ScheduleBuilder builder(
        startDate_,
        endDate_,
        accrualPeriod_,
        stubRule_
    );

    const auto schedule = builder.build();


    cashflows_.reserve(schedule.size());

    for (const auto& [accrualStart, accrualEnd] : schedule.accruals()) {

        // Date de paiement = fin d'accrual + paymentDelay (jours civils)
        Date rawPaymentDate = 
            shiftBusinessDays(accrualEnd, paymentDelay_, calendar_);

        Date paymentDate =
            adjustDate(rawPaymentDate, calendar_, bdc_);


        Cashflow cf(
            paymentDate,
            std::nullopt,
            accrualStart,
            accrualEnd,
            currency_,
            notional_,
            payReceive_,
            coupon_
        );

        cashflows_.push_back(std::move(cf));
    }

    std::sort(
        cashflows_.begin(),
        cashflows_.end(),
        [](const Cashflow& a, const Cashflow& b) {
            return a.paymentDate() < b.paymentDate();
        }
    );

    validateInvariants();
}

} // namespace qris::contract
