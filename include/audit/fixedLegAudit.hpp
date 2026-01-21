#pragma once

#include <iosfwd>

#include "contract/leg/fixedLeg.hpp"

namespace qris::audit {

/**
 * @brief Affiche un rapport d’audit contractuel d’une FixedLeg.
 *
 * Aucun calcul financier.
 * Lecture seule des cashflows générés.
 */
void dumpFixedLeg(
    const qris::contract::FixedLeg& leg,
    std::ostream& out
);

} // namespace qris::audit
