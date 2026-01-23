#include "contract/leg/floatingLeg.hpp"

#include "time/scheduleBuilder.hpp"
#include "time/businessDayConvention.hpp"
#include "time/time_arithmetic.hpp"

#include <algorithm>
#include <stdexcept>

namespace qris::contract {

using qris::time::ScheduleBuilder;
using qris::time::adjustDate;

FloatingLeg::FloatingLeg(
    qris::time::Date startDate,
    qris::time::Date endDate,
    qris::time::Period accrualPeriod,
    qris::time::Calendar calendar,
    qris::time::BusinessDayConvention bdc,
    qris::time::StubRule stubRule,
    int paymentDelay,
    int fixingLag,
    double notional,
    qris::core::Currency currency,
    qris::core::PayReceive payReceive,
    std::shared_ptr<const qris::core::RateIndex> index,
    double spread,
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
, coupon_(std::make_shared<FloatingCoupon>(index, spread, dayCount))
{
    // ----------------------------
    // Validations locales
    // ----------------------------

    if (!index) {
        throw std::invalid_argument(
            "FloatingLeg: RateIndex must not be null");
    }

    // ----------------------------
    // Construction du schedule
    // ----------------------------

    ScheduleBuilder builder(
        startDate_,
        endDate_,
        accrualPeriod_,
        stubRule_
    );

    const auto schedule = builder.build();

    cashflows_.reserve(schedule.size());

    for (const auto& [accrualStart, accrualEnd] : schedule.accruals()) {

        // ----------------------------
        // Fixing date
        // ----------------------------
        qris::time::Date fixingDate = 
            qris::time::shiftBusinessDays(accrualStart, -fixingLag, index->fixingCalendar());

        // ----------------------------
        // Payment date
        // ----------------------------
        Date rawPaymentDate = 
            shiftBusinessDays(accrualEnd, paymentDelay_, calendar_);

        Date paymentDate =
            adjustDate(rawPaymentDate, calendar_, bdc_);


        Cashflow cf(
            paymentDate,
            fixingDate,
            accrualStart,
            accrualEnd,
            currency_,
            notional_,
            payReceive_,
            coupon_
        );

        cashflows_.push_back(std::move(cf));
    }

    // ----------------------------
    // Ordonnancement & invariants
    // ----------------------------

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
