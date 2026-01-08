#pragma once

#include <vector>
#include <utility>
#include <cstddef>
#include <stdexcept>

#include "time/date.hpp"

namespace qris::time {

class ScheduleBuilder;  // seule autorité de construction

/**
 * @brief Échéancier contractuel interne.
 *
 * Structure technique transitoire représentant une suite ordonnée
 * de périodes d’accrual (startDate, endDate).
 *
 * - Objet passif
 * - Immuable après construction
 * - Non persistant
 * - Aucun calcul
 *
 * Le Schedule est destiné à être consommé immédiatement (ex: par une Leg)
 * puis détruit.
 */
class Schedule {
public:
    using AccrualPeriod = std::pair<Date, Date>;

    /// Accès en lecture seule aux périodes (itération uniquement)
    const std::vector<AccrualPeriod>& accruals() const noexcept {
        return accruals_;
    }

    /// Nombre de périodes d’accrual
    std::size_t size() const noexcept {
        return accruals_.size();
    }

private:
    std::vector<AccrualPeriod> accruals_;

    /// Construction restreinte au ScheduleBuilder
    explicit Schedule(std::vector<AccrualPeriod> accruals)
        : accruals_(std::move(accruals))
    {
        validate();
    }

    /// Invariants structurels minimaux
    void validate() const {
        if (accruals_.empty()) {
            throw std::logic_error(
                "Schedule: empty accrual schedule is not allowed"
            );
        }

        for (std::size_t i = 0; i < accruals_.size(); ++i) {
            const auto& [start, end] = accruals_[i];

            if (!(start < end)) {
                throw std::logic_error(
                    "Schedule: accrual period has non-positive length"
                );
            }

            if (i > 0 && accruals_[i - 1].second != start) {
                throw std::logic_error(
                    "Schedule: accrual periods are not contiguous"
                );
            }
        }
    }

    friend class ScheduleBuilder;
};

} // namespace qris::time
