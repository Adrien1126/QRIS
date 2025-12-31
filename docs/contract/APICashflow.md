# Cashflow — API C++ cible (contrat figé)

## 1. Positionnement dans l’architecture

* `Cashflow` appartient au **module contractuel**
* Il est **en dessous** des instruments
* Il est **au-dessus** du pricing
* Il est **indépendant du marché**

```
Date / Calendar / Convention
        ↓
     Cashflow
        ↓
        Leg
        ↓
    Instrument
        ↓
   Pricing Engine
```

---

## 2. Nature de la classe

### 2.1 Classe concrète (pas abstraite)

Décision figée :

> **`Cashflow` est une classe concrète.**

Justification :

* tous les cashflows partagent un **socle contractuel commun**
* la spécialisation (fixed / floating / optional) est **décrite par des données**, pas par du polymorphisme
* évite une hiérarchie lourde et rigide
* facilite la sérialisation, le stockage DB, les tests

➡️ **Pas de `FixedCashflow`, `FloatingCashflow` en héritage**

---

### 2.2 Immutabilité stricte

* Tous les membres sont :

  * initialisés au constructeur
  * accessibles uniquement en lecture
* Aucun setter
* Aucune méthode mutante

---

## 3. Types fondamentaux utilisés

Avant même la classe, on fige certains types **forts**.

### 3.1 Direction du cashflow

```cpp
enum class PayReceive {
    Pay,
    Receive
};
```

➡️ Pas de signe implicite
➡️ Le signe est **une donnée contractuelle**

---

### 3.2 Type de coupon

```cpp
enum class CouponType {
    Fixed,
    Floating,
    Capped,
    Floored,
    Collared
};
```

➡️ Permet l’extension sans changer l’API
➡️ Le pricing décidera quoi faire

---

## 4. Données portées par Cashflow (figées)

### 4.1 Données temporelles

Obligatoires :

* `paymentDate`

Conditionnelles :

* `accrualStartDate`
* `accrualEndDate`
* `fixingDate`

Règle :

* **toutes les dates sont déjà ajustées**
* aucune convention appliquée ici

---

### 4.2 Données financières

Obligatoires :

* `currency`
* `notional`
* `payReceive`

---

### 4.3 Données de coupon

Selon le type :

* taux fixe (si Fixed)
* index + spread (si Floating)
* bornes optionnelles (cap / floor)

---

### 4.4 Conventions locales

* `DayCountConvention` **appartient au cashflow**
* jamais calculée ici
* uniquement exposée

---

## 5. API publique — signatures cibles

### 5.1 Accesseurs temporels

```cpp
const Date& paymentDate() const;

const Date& accrualStartDate() const;
const Date& accrualEndDate() const;

const Date& fixingDate() const;
```

> ⚠️ Pour les cashflows sans fixing / accrual, la date n’existe pas
> → décision : **std::optional<Date>**

---

### 5.2 Accesseurs contractuels

```cpp
double notional() const;
PayReceive direction() const;
const std::string& currency() const;
```

---

### 5.3 Accesseurs coupon

```cpp
CouponType couponType() const;

double fixedRate() const;            // si Fixed
double spread() const;               // si Floating

const std::string& indexName() const; // ex: "EURIBOR_3M"
```

➡️ Les préconditions sont **contractuelles**, pas runtime-heavy
➡️ Le pricing sait quoi appeler selon le type

---

### 5.4 Accesseur convention

```cpp
DayCountConvention dayCountConvention() const;
```

---

## 6. Méthodes explicitement interdites

Il **n’existera jamais** dans `Cashflow` :

```cpp
double amount() const;
double yearFraction() const;
double npv(...) const;
double discount(...) const;
```

➡️ Toute tentative de calcul est **hors périmètre**

---

## 7. Constructeurs (contrat)

### 7.1 Constructeur explicite et strict

* Tous les champs nécessaires sont fournis
* Validation contractuelle immédiate
* Politique *fail-fast*

Conceptuellement :

```cpp
Cashflow(
    paymentDate,
    accrualStartDate,
    accrualEndDate,
    fixingDate,
    notional,
    currency,
    direction,
    couponDescription,
    dayCountConvention
);
```

➡️ La **Leg est responsable** de fournir des dates cohérentes

---

## 8. Égalité et identité

Décision importante :

* Deux cashflows sont égaux si :

  * toutes leurs données contractuelles sont égales

➡️ Pas d’ID technique ici
➡️ L’ID DB viendra **à l’extérieur**

---

## 9. Rôle du Cashflow dans le pricing

Le pricing engine :

* lit les données du cashflow
* applique :

  * conventions
  * marché
  * courbes
* produit une valeur

➡️ **Flux de dépendance unidirectionnel**

---

## 10. Résumé des choix figés

| Décision              | Choix |
| --------------------- | ----- |
| Cashflow abstrait     | ❌ Non |
| Héritage              | ❌ Non |
| Immuable              | ✅ Oui |
| Connaît le marché     | ❌ Non |
| Connaît day count     | ✅ Oui |
| Calcule quelque chose | ❌ Non |
| Testable seul         | ✅ Oui |
| Sérialisable          | ✅ Oui |


