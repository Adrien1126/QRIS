# API cible — `Leg` (revue conceptuelle)

## 1. Statut de `Leg`

* `Leg` est une **classe abstraite**
* **non instanciable**
* appartenant à la **couche contractuelle**
* responsable de la **génération complète des cashflows**

👉 Toute `Leg` concrète doit être **entièrement valide après construction**.

---

## 2. Responsabilités exactes de `Leg`

### 2.1 Ce que `Leg` FAIT

`Leg` :

* porte **tous les paramètres contractuels communs**,
* construit le **schedule contractuel**,
* applique :

  * calendrier,
  * business day convention,
  * stub rule,
  * payment delay,
  * fixing lag (si applicable),
* génère **l’intégralité des `Cashflow`**,
* stocke les cashflows de manière **ordonnée et immuable**,
* valide **tous les invariants globaux**.

---

### 2.2 Ce que `Leg` NE FAIT PAS

`Leg` :

* ❌ ne calcule aucun montant,
* ❌ ne valorise rien,
* ❌ ne connaît aucune courbe,
* ❌ ne dépend d’aucune donnée de marché,
* ❌ ne modifie jamais ses cashflows après construction.

---

## 3. Données portées par `Leg` (conceptuellement)

### 3.1 Paramètres temporels

Une `Leg` doit porter **au minimum** :

* `startDate`
* `endDate`
* `paymentFrequency`
* `calendar`
* `businessDayConvention`
* `stubRule`
* `paymentDelay`

Et **optionnellement** :

* `fixingLag` (utilisé uniquement par certaines legs)

👉 Ces paramètres définissent **le contrat temporel de la leg**.

---

### 3.2 Paramètres financiers

Communs à tous les cashflows :

* `notional`
* `currency`
* `direction` (*Pay / Receive*)

---

### 3.3 Stockage interne

* Une `Leg` **possède** ses cashflows
* Stockage conceptuel :

  * collection ordonnée
  * stable
  * non modifiable

👉 Le choix exact du conteneur est un **détail d’implémentation**, pas un choix d’API.

---

## 4. Relation avec les classes dérivées

### 4.1 Point de variation principal

Le **seul point de variation** entre les legs est :

> *comment un cashflow individuel est construit*
> (coupon + fixing date éventuelle)

👉 Cela doit être le **seul point virtualisé**.

---

### 4.2 Responsabilités de `Leg` abstraite

`Leg` abstraite :

* construit le schedule,
* itère sur les périodes,
* applique les conventions,
* appelle un **point d’extension** pour :

  * construire le coupon,
  * déterminer la fixing date (si applicable),
* assemble les `Cashflow`.

---

### 4.3 Responsabilités des classes dérivées

Les classes concrètes (`FixedLeg`, `FloatingLeg`) :

* fournissent la logique spécifique pour :

  * le type de coupon,
  * les paramètres du coupon,
  * la fixing date (ou son absence).

👉 Elles **ne touchent pas** :

* au calendrier,
* au schedule,
* aux conventions globales.

---

## 5. API publique exposée par `Leg`

### 5.1 Accès aux cashflows

Une `Leg` expose :

* un accès **en lecture seule** à ses cashflows
* garantissant :

  * l’ordre chronologique,
  * la stabilité des objets,
  * l’absence de recalcul.

Conceptuellement :

* vue constante,
* itérable,
* sans copie.

---

### 5.2 Accès aux paramètres contractuels

Une `Leg` peut exposer (lecture seule) :

* start date
* end date
* fréquence
* notional
* currency
* direction

👉 Ces getters servent :

* à l’audit,
* au reporting,
* au debug,
* à la construction d’instruments.

---

## 6. Invariants garantis par `Leg`

Dès la fin du constructeur :

* dates cohérentes (`start < end`)
* fréquence compatible avec les dates
* notionnel strictement positif
* au moins un cashflow généré
* cashflows :

  * ordonnés,
  * valides,
  * aux dates ajustées
* cohérence entre :

  * fixing lag et type de leg,
  * payment delay et calendrier

👉 **Aucun objet `Leg` invalide ne peut exister.**

---

## 7. Erreurs et stratégie d’échec

* Toute violation contractuelle :

  * **exception immédiate à la construction**
* Aucune tolérance partielle
* Aucun état intermédiaire observable

👉 Si une `Leg` existe, elle est **contractuellement saine**.

---

## 8. Extensibilité prévue

Cette API permet :

* d’ajouter de nouveaux types de legs **sans modifier `Leg`**,
* d’introduire :

  * amortizing legs,
  * inflation legs,
  * CMS legs,
  * exotic legs,
* de composer des instruments complexes sans refactor.

---

## 9. Comparaison avec QuantLib (volontairement assumée)

| QuantLib            | Ton moteur                   |
| ------------------- | ---------------------------- |
| `Schedule` externe  | Schedule intégré à `Leg`     |
| `Leg` = vector      | `Leg` = entité contractuelle |
| Génération éclatée  | Génération centralisée       |
| Mutabilité possible | Immuabilité stricte          |

👉 **Ton choix est plus sûr, plus explicite et plus auditable.**

---

## 10. Résumé exécutif

* `Leg` est une **entité contractuelle abstraite**
* Elle **génère et possède** ses cashflows
* Elle applique **toutes les conventions**
* Elle expose une **vue stable et constante**
* Elle ne fait **aucun calcul de prix**
* Elle est le **pilier contractuel des instruments**

