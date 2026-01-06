#pragma once

namespace qris::time {

/**
 * @brief Unités temporelles fondamentales supportées par Period.
 *
 * PeriodUnit représente les unités de temps contractuelles
 * utilisables pour définir une Period (durée abstraite).
 *
 * Ces unités sont purement descriptives :
 *  - elles ne portent aucune logique calendaire,
 *  - elles ne réalisent aucun calcul,
 *  - elles ne dépendent d'aucune date ni calendrier.
 *
 * Les périodicités de marché (Quarterly, SemiAnnual, etc.)
 * sont des alias sémantiques construits à partir de ces unités
 * (ex: Quarterly = 3 * Months).
 */
enum class PeriodUnit {
    D,  ///< Days
    W,  ///< Weeks
    M,  ///< Months
    Y   ///< Years
};

} // namespace qris::time
