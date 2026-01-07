# API cible — `Leg` (version alignée)

## 1. Nature et statut de `Leg`

* `Leg` est une **classe abstraite**
* **non instanciable directement**
* appartenant à la **couche contractuelle**
* représentant une **suite contractuelle homogène de cashflows**

👉 Une instance de `Leg` est **toujours complète, cohérente et finale** dès la fin de sa construction.

---

## 2. Responsabilité centrale de `Leg`

`Leg` est responsable de **l’orchestration complète de la génération des cashflows**, mais **pas de leur valorisation**.

Concrètement, `Leg` :

1. reçoit les **paramètres contractuels globaux**,
2. génère un **échéancier contractuel**,
3. applique les **conventions temporelles et décalages**,
4. instancie **l’intégralité des cashflows**,
5. expose ces cashflows en **lecture seule**.

---

## 3. Paramètres contractuels portés par `Leg`

### 3.1 Paramètres temporels (obligatoires)

Une `Leg` porte :

* `startDate`
* `endDate`
* `paymentFrequency` (`Period`)
* `calendar`
* `businessDayConvention`
* `stubRule`
* `paymentDelay`

Ces paramètres définissent **le contrat temporel global** de la leg.

---

### 3.2 Paramètres temporels conditionnels

Selon le type de leg :

* `fixingLag`

  * présent uniquement pour certaines legs (ex : flottantes)
  * absent ou nul pour les autres

👉 La validité de ces paramètres est **vérifiée par la `Leg` abstraite**.

---

### 3.3 Paramètres financiers communs

Communs à tous les cashflows :

* `notional`
* `currency`
* `direction` (*Pay / Receive*)

---

## 4. Génération du schedule (vue API)

### 4.1 Positionnement architectural

La génération de l’échéancier est :

* **orchestrée par `Leg`**,
* mais **déportée conceptuellement** dans un composant dédié (ex : `ScheduleBuilder`).

👉 Du point de vue de l’API de `Leg` :

* le *comment* est un détail interne,
* le *quoi* est contractuel et visible.

---

### 4.2 Résultat attendu

La génération produit :

* une séquence ordonnée de périodes d’accrual,
* couvrant `[startDate, endDate]`,
* respectant :

  * fréquence,
  * règle de stub,
  * conventions calendaires.

---

## 5. Point de variation unique (abstraction clé)

### 5.1 Principe fondamental

Le **seul point de variation** entre les différents types de `Leg` est :

> **la manière de construire un cashflow individuel à partir d’une période donnée**

Tout le reste est **mutualisé**.

---

### 5.2 Rôle de la classe `Leg` abstraite

La `Leg` abstraite :

* génère le schedule,
* itère sur les périodes,
* applique :

  * payment delay,
  * fixing lag (si applicable),
* valide les invariants globaux,
* appelle un **hook abstrait** pour chaque période afin de :

  * construire le coupon,
  * déterminer la fixing date éventuelle.

---

### 5.3 Rôle des classes dérivées

Les classes concrètes (`FixedLeg`, `FloatingLeg`, etc.) :

* implémentent **uniquement** :

  * la logique de création du coupon,
  * la logique spécifique de fixing (ou son absence).

Elles **n’interviennent jamais** sur :

* la génération des dates,
* les conventions calendaires,
* l’ordre des cashflows,
* les paramètres globaux.

---

## 6. Stockage et exposition des cashflows

### 6.1 Stockage interne

Une `Leg` :

* **possède** ses `Cashflow`,
* les stocke dans une collection :

  * ordonnée chronologiquement,
  * stable,
  * non modifiable après construction.

Le type exact du conteneur est un **détail d’implémentation**.

---

### 6.2 API publique d’accès

Une `Leg` expose :

* une **vue constante** sur ses cashflows,
* itérable,
* sans copie,
* sans possibilité de mutation.

---

## 7. API publique complémentaire

Une `Leg` peut exposer en lecture seule :

* `startDate`
* `endDate`
* `paymentFrequency`
* `notional`
* `currency`
* `direction`

Ces accesseurs servent à :

* l’audit contractuel,
* le reporting,
* la construction d’instruments,
* le debug.

---

## 8. Invariants garantis

À la fin de la construction :

* `startDate < endDate`
* fréquence compatible avec l’intervalle
* `notional > 0`
* au moins un cashflow généré
* cashflows :

  * ordonnés,
  * contractuellement valides,
  * aux dates ajustées
* cohérence :

  * fixingLag ↔ type de leg
  * paymentDelay ↔ calendrier

👉 **Aucune instance invalide de `Leg` ne peut exister.**

---

## 9. Stratégie d’échec

* Toute incohérence contractuelle :

  * exception immédiate à la construction
* Aucun état intermédiaire observable
* Aucune correction implicite

---

## 10. Résumé exécutif (API)

* `Leg` est une **classe abstraite**
* Elle orchestre la génération complète des cashflows
* Elle mutualise toute la logique temporelle
* Elle virtualise **un seul point de variation**
* Elle expose une vue stable et constante
* Elle ne fait **aucun calcul de pricing**

