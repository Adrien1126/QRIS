#pragma once

#include <iosfwd>

#include "contract/leg/floatingLeg.hpp"

namespace qris::audit {

/**
 * @brief Affiche un rapport d’audit contractuel d’une FloatingLeg.
 *
 * Aucun calcul financier.
 * Lecture seule des cashflows générés.
 * Le rapport expose :
 *  - les périodes d’accrual,
 *  - les fixing dates,
 *  - les payment dates,
 *  - les paramètres d’indexation.
 */
void dumpFloatingLeg(
    const qris::contract::FloatingLeg& leg,
    std::ostream& out
);

} // namespace qris::audit
