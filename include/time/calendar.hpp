#pragma once

#include <string>
#include <vector>

#include "time/date.hpp"

namespace qris::time {

/**
 * @brief Calendrier de jours ouvrés (objet métier immuable).
 *
 * Le Calendar détermine si une date civile est ouvrée ou non
 * selon :
 *  - la règle standard de week-end (samedi / dimanche),
 *  - une liste explicite de jours fériés.
 *
 * Il ne modifie jamais les dates et ne connaît aucune convention.
 */
class Calendar {

private:
    std::string name_;
    std::vector<Date> holidays_;

public:
    /**
     * @brief Construit un calendrier immuable.
     *
     * @param name Nom du calendrier (ex: "TARGET")
     * @param holidays Liste explicite des jours fériés
     */
    Calendar(std::string name,
             std::vector<Date> holidays) : name_(name), holidays_(holidays) {}

    /**
     * @brief Indique si une date est un jour ouvré.
     */
    bool isBusinessDay(const Date& date) const;

    /**
     * @brief Nom du calendrier.
     */
    const std::string& name() const {return name_;}
};

} // namespace qris::time