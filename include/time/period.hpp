#pragma once

#include "time/periodUnit.hpp"

namespace qris::time {
/**
 * @brief Période temporelle contractuelle abstraite.
 *
 * Period représente une durée de temps exprimée comme une combinaison
 * d’un multiplicateur strictement positif et d’une unité temporelle
 * (Days, Weeks, Months ou Years).
 *
 * Il s’agit d’un type fondamental du module time, utilisé pour décrire
 * des périodicités contractuelles (fréquences de paiement, maturités,
 * découpages temporels), indépendamment de toute date concrète.
 *
 * Responsabilités exclues :
 *  - Period ne manipule aucune Date.
 *  - Period ne connaît aucun calendrier.
 *  - Period n’effectue aucun ajustement de jours ouvrés.
 *  - Period ne réalise aucun calcul financier.
 *
 * Invariants forts :
 *  - une instance de Period est toujours valide par construction ;
 *  - le multiplicateur est strictement positif ;
 *  - l’unité temporelle est toujours connue et supportée ;
 *  - l’objet est totalement immuable après construction.
 *
 * Conventions contractuelles :
 *  - une Period peut être exprimée en D, W, M ou Y ;
 *  - toute combinaison valide est autorisée (ex : 7D, 3M, 18M, 2Y) ;
 *  - certaines représentations sont contractuellement équivalentes
 *    (ex : 3M ≡ Quarterly, 6M ≡ SemiAnnual, 12M ≡ 1Y).
 *
 * L’équivalence entre périodes est contractuelle et descriptive ;
 * elle ne doit pas être interprétée comme une égalité calendaire
 * universelle.
 */

class Period {
private: 
    int multiplier_;
    PeriodUnit unit_; 
public : 
    Period(int multiplier, PeriodUnit unit); 
    int multiplier() const noexcept{return multiplier_;}
    PeriodUnit unit() const noexcept{return unit_;}
    bool operator==(const Period& p) const noexcept; 
    bool operator<(const Period& p) const;
    bool operator>(const Period& p) const;
}; 
}