#pragma once
#include "time/date.hpp"
#include "time/period.hpp"
#include "time/calendar.hpp"

namespace qris::time {
Date addPeriod(const Date& d, const Period& p); 
Date subtractPeriod(const Date& d, const Period& p); 

// -----------------------------------------------------------------------------
// Arithmétique en jours ouvrés
// -----------------------------------------------------------------------------

/**
 * @brief Décale une date d'un nombre de jours ouvrés selon un calendrier.
 *
 * @param d            Date de référence
 * @param businessDays Nombre de jours ouvrés (positif ou négatif)
 * @param calendar     Calendrier de référence
 *
 * @return Date résultante après décalage
 *
 * @note
 * - businessDays > 0 : avance dans le temps
 * - businessDays < 0 : recule dans le temps
 * - businessDays = 0 : retourne d
 *
 * @note
 * Le décompte se fait strictement sur les jours ouvrés du calendrier fourni.
 */
Date shiftBusinessDays(
    const Date& d,
    int businessDays,
    const Calendar& calendar
);
}