#include <stdexcept>

#include "time/scheduleBuilder.hpp"
#include "time/time_arithmetic.hpp"

namespace qris::time {
ScheduleBuilder::ScheduleBuilder(Date startDate, Date endDate, Period period, StubRule stubRule)
    : startDate_(startDate), endDate_(endDate), period_(period), stubRule_(stubRule)
{
    if (startDate >= endDate) {
        throw std::invalid_argument("ScheduleBuilder: endDate must be strictly after startDate");
    }
}

Schedule ScheduleBuilder::build() const
{
    if (stubRule_ == StubRule::ShortFront || stubRule_ == StubRule::LongFront) {
        auto accruals = buildBackward();
        validateSchedule(accruals);
        return Schedule(std::move(accruals));
    }

    if (stubRule_ == StubRule::ShortBack || stubRule_ == StubRule::LongBack) {
        auto accruals = buildForward();
        validateSchedule(accruals);
        return Schedule(std::move(accruals));
    }

    throw std::logic_error("ScheduleBuilder::build: unsupported StubRule");
}


std::vector<Schedule::AccrualPeriod>
ScheduleBuilder::buildForward() const
{
    std::vector<Schedule::AccrualPeriod> accruals;

    Date start = startDate_;
    Date next  = addPeriod(start, period_);

    // Génération des périodes complètes
    while (next < endDate_) {
        accruals.emplace_back(start, next);
        start = next;
        next  = addPeriod(start, period_);
    }

    // À ce stade :
    // start < endDate_ <= next

    // Gestion du stub back
    if (start < endDate_) {

        if (stubRule_ == StubRule::ShortBack) {
            // Stub court : dernière période raccourcie
            accruals.emplace_back(start, endDate_);
        }
        else if (stubRule_ == StubRule::LongBack) {
            // Stub long : on fusionne avec la période précédente
            if (accruals.empty()) {
                // Cas particulier : une seule période
                accruals.emplace_back(startDate_, endDate_);
            } else {
                auto& last = accruals.back();
                last.second = endDate_;
            }
        }
        else {
            throw std::logic_error(
                "ScheduleBuilder::buildForward: invalid StubRule for forward generation"
            );
        }
    }

    return accruals;
}

std::vector<Schedule::AccrualPeriod>
ScheduleBuilder::buildBackward() const
{
    std::vector<Schedule::AccrualPeriod> accruals;

    Date end   = endDate_;
    Date start = subtractPeriod(end, period_);

    // Génération backward des périodes complètes
    while (start > startDate_) {
        accruals.emplace_back(start, end);
        end   = start;
        start = subtractPeriod(end, period_);
    }

    // À ce stade :
    // start <= startDate_ < end

    if (startDate_ < end) {

        if (stubRule_ == StubRule::ShortFront) {
            // Stub court en front
            accruals.emplace_back(startDate_, end);
        }
        else if (stubRule_ == StubRule::LongFront) {
            // Stub long : fusion avec la période suivante
            if (accruals.empty()) {
                // Cas d'une seule période
                accruals.emplace_back(startDate_, endDate_);
            } else {
                // La dernière période ajoutée correspond au front
                auto& last = accruals.back();
                last.first = startDate_;
            }
        }
        else {
            throw std::logic_error(
                "ScheduleBuilder::buildBackward: invalid StubRule for backward generation"
            );
        }
    }

    // Les périodes ont été construites en ordre inverse
    std::reverse(accruals.begin(), accruals.end());

    return accruals;
}

void ScheduleBuilder::validateSchedule(
    const std::vector<Schedule::AccrualPeriod>& accruals
) const
{
    if (accruals.empty()) {
        throw std::logic_error(
            "ScheduleBuilder::validateSchedule: generated schedule is empty"
        );
    }

    if (accruals.front().first != startDate_) {
        throw std::logic_error(
            "ScheduleBuilder::validateSchedule: "
            "first accrual period does not start at schedule startDate"
        );
    }

    if (accruals.back().second != endDate_) {
        throw std::logic_error(
            "ScheduleBuilder::validateSchedule: "
            "last accrual period does not end at schedule endDate"
        );
    }

    for (std::size_t i = 0; i < accruals.size(); ++i) {
        const auto& [start, end] = accruals[i];

        if (!(start < end)) {
            throw std::logic_error(
                "ScheduleBuilder::validateSchedule: "
                "invalid accrual period with non-positive length (start >= end)"
            );
        }

        if (i > 0 && accruals[i - 1].second != start) {
            throw std::logic_error(
                "ScheduleBuilder::validateSchedule: "
                "schedule contains a gap or overlap between consecutive accrual periods"
            );
        }
    }
}

}
