#pragma once

#include <vector>

#include "time/date.hpp"
#include "time/period.hpp"
#include "time/stubRule.hpp"
#include "time/schedule.hpp"

namespace qris::time {

/**
 * @brief Générateur contractuel de schedule d'accrual.
 *
 * ScheduleBuilder est un objet utilitaire interne, à durée de vie courte,
 * responsable de la génération des périodes d'accrual d'une Leg.
 *
 * Il opère exclusivement sur des dates contractuelles brutes :
 *  - aucune logique de calendrier,
 *  - aucun ajustement de business day,
 *  - aucune notion de paiement ou de fixing.
 *
 * Toute logique d'ajustement est volontairement exclue et relève de la Leg.
 */
class ScheduleBuilder {
public:
    /**
     * @brief Constructeur du ScheduleBuilder.
     *
     * @param startDate Date de début contractuelle de la leg
     * @param endDate   Date de fin contractuelle de la leg
     * @param period    Périodicité contractuelle (ex : 3M, 6M)
     * @param stubRule  Règle de gestion du stub (front/back, short/long)
     *
     * Invariants :
     *  - startDate < endDate (strict)
     *  - period valide
     *
     * Toute violation entraîne une exception immédiate.
     */
    ScheduleBuilder(
        Date startDate,
        Date endDate,
        Period period,
        StubRule stubRule
    );

    /**
     * @brief Génère le schedule d'accrual.
     *
     * @return Schedule immuable contenant une collection ordonnée
     *         de paires (accrualStart, accrualEnd).
     *
     * Le schedule retourné peut être vide :
     *  - si (endDate - startDate) < period
     *  - ou si startDate == endDate
     *
     * @throws std::invalid_argument en cas d'incohérence contractuelle.
     */
    Schedule build() const;

private:
    // Paramètres contractuels immuables
    Date startDate_;
    Date endDate_;
    Period period_;
    StubRule stubRule_;

private:
    /**
     * @brief Génération forward (stub en back).
     *
     * Génère une grille de dates croissantes à partir de startDate.
     */
    std::vector<Schedule::AccrualPeriod> buildForward() const;

    /**
     * @brief Génération backward (stub en front).
     *
     * Génère une grille de dates décroissantes à partir de endDate.
     */
    std::vector<Schedule::AccrualPeriod> buildBackward() const;

    /**
     * @brief Validation finale du schedule généré.
     *
     * Vérifie :
     *  - cohérence des accruals
     *  - bornes exactes (startDate / endDate)
     *  - unicité et validité du stub
     */
    void validateSchedule(
        const std::vector<Schedule::AccrualPeriod>& accruals
    ) const;
};

} // namespace qris::time
