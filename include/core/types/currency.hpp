#pragma once

#include <string>

namespace qris::core {

/**
 * @brief Devise contractuelle (identifiée par code ISO).
 *
 * Objet métier immuable représentant la devise de règlement
 * d’un contrat ou d’un flux financier.
 *
 * - Identité unique : code ISO (ex: "EUR", "USD")
 * - Pas de logique de marché
 * - Pas de conversion
 * - Pas d’I/O
 */
class Currency {
private:
    std::string isoCode_;
    
public:
    /**
     * @brief Construit une devise à partir de son code ISO.
     *
     * @param isoCode Code ISO (ex: "EUR", "usd")
     *
     * @throws std::invalid_argument si le code est invalide
     */
    explicit Currency(std::string isoCode);

    /**
     * @brief Retourne le code ISO normalisé (uppercase).
     */
    const std::string& isoCode() const noexcept;

    /**
     * @brief Comparaison par identité ISO.
     */
    bool operator==(const Currency& other) const noexcept;
    bool operator!=(const Currency& other) const noexcept;

    /**
     * @brief Représentation textuelle.
     */
    std::string toString() const;
};

} // namespace qris::core
