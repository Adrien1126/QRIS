#pragma once

namespace qris::time {

/**
 * @brief Jour de la semaine (calendrier civil).
 *
 * Représente un jour de la semaine selon le calendrier grégorien.
 * Ce type est purement civil et indépendant de toute logique financière.
 *
 * Convention :
 *  - Monday correspond au premier jour de la semaine
 *  - L’ordre est strictement séquentiel
 *
 * Ce type est utilisé par :
 *  - Date : pour exposer le jour de la semaine
 *  - Calendar : pour déterminer l’ouvrabilité
 */
enum class WeekDay {
    Monday = 0,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

} // namespace qris::time
