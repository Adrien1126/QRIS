#pragma once

#include <stdexcept>
#include <string>
#include "time/weekday.hpp"


namespace qris::time {
/**
 * @brief Date civile immuable (calendrier grégorien strict)
 *
 * Représente une date civile valide, indépendante de toute logique financière
 * (calendriers, conventions de marché, day count, etc.).
 *
 * Invariants forts :
 *  - une instance de Date est TOUJOURS valide
 *  - aucune date invalide ne peut exister par construction
 *  - la classe est immuable après construction
 *
 * Conventions :
 *  - calendrier grégorien strict (1900 n'est PAS bissextile)
 *  - domaine supporté : dates >= 01/01/1900
 *  - représentation canonique interne via un "serial number"
 *    avec origine : 01/01/1900 -> serial = 1
 *
 * Toute logique financière (jours ouvrés, conventions, day count fraction)
 * est volontairement EXCLUE de cette classe.
 */
class Date {
private:
    // --- État interne (immuable après construction) ---
    int day_;
    int month_;
    int year_;

    // ---------------------------------------------------------------------
    // Invariants fondamentaux (logique civile pure)
    // ---------------------------------------------------------------------

    /**
     * @brief Indique si une année est bissextile (calendrier grégorien).
     *
     * Règle :
     *  - divisible par 4
     *  - sauf si divisible par 100
     *  - sauf si divisible par 400
     */
    static bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    /**
     * @brief Nombre de jours dans un mois donné pour une année donnée.
     *
     * @throws std::invalid_argument si le mois est hors [1,12]
     */
    static int daysInMonth(int month, int year) {
        if (month < 1 || month > 12) {
            throw std::invalid_argument("Month must be in [1,12]");
        }

        switch (month) {
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return isLeapYear(year) ? 29 : 28;
        default:
            return 31;
        }
    }

    /**
     * @brief Vérifie la validité globale d'une date civile.
     *
     * Politique : fail-fast.
     * Si la date est invalide, la construction doit échouer immédiatement.
     */
    static void validate(int day, int month, int year) {
        if (year < 1900) {
            throw std::invalid_argument("Year must be >= 1900");
        }

        if (month < 1 || month > 12) {
            throw std::invalid_argument("Month must be in [1,12]");
        }

        int maxDay = daysInMonth(month, year);
        if (day < 1 || day > maxDay) {
            throw std::invalid_argument("Invalid day for given month/year");
        }
    }

public:
    // ---------------------------------------------------------------------
    // Construction
    // ---------------------------------------------------------------------

    /**
     * @brief Construit une date civile valide.
     *
     * @throws std::invalid_argument si la date est invalide
     */
    Date(int day, int month, int year)
        : day_(day), month_(month), year_(year)
    {
        validate(day_, month_, year_);
    }

    /**
     * @brief Construit une date à partir d'un serial number.
     *
     * Convention :
     *  - 01/01/1900 -> serial = 1
     *  - calendrier grégorien strict
     *
     * @throws std::invalid_argument si le serial est invalide
     */
    explicit Date(int serialNumber);

    // ---------------------------------------------------------------------
    // Accesseurs (observation pure)
    // ---------------------------------------------------------------------

    int day() const   { return day_; }
    int month() const { return month_; }
    int year() const  { return year_; }

    // ---------------------------------------------------------------------
    // Comparaisons (ordre total strict)
    // ---------------------------------------------------------------------

    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;

    // ---------------------------------------------------------------------
    // Arithmétique civile (jours civils uniquement)
    // ---------------------------------------------------------------------

    /**
     * @brief Retourne une nouvelle date obtenue par ajout de jours civils.
     *
     * Aucun calendrier ni convention de marché n'est appliqué.
     */
    Date addDays(int days) const;

    /**
     * @brief Différence en jours civils entre deux dates.
     *
     * Convention :
     *  - this.diff(other) = this_serial - other_serial
     */
    int diff(const Date& other) const;

    // ---------------------------------------------------------------------
    // Représentation canonique
    // ---------------------------------------------------------------------

    /**
     * @brief Convertit la date en serial number.
     *
     * Convention :
     *  - 01/01/1900 -> serial = 1
     *  - utilisé comme représentation canonique interne
     */
    int dateToSerialNumber() const;

    // ---------------------------------------------------------------------
    // Représentation textuelle
    // ---------------------------------------------------------------------

    /**
     * @brief Représentation textuelle ISO (YYYY-MM-DD).
     *
     * Utilisée pour le debug, les logs et l'affichage.
     */
    std::string toString() const;
};

std::ostream& operator<<(std::ostream& os, const Date& d);

} // namespace qris::time
 
