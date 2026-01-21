# Fixed Leg — Contractual Audit Documentation

## 1. Objectif de l’audit

Ce document décrit la **piste d’audit contractuelle** mise en place pour valider la cohérence temporelle et financière d’un **Fixed Leg** dans le moteur QRIS.

L’objectif n’est **pas le pricing**, mais la **vérification explicite et traçable** que :

- les échéanciers sont correctement générés,
- les conventions de marché sont correctement appliquées,
- les invariants contractuels sont respectés ou violés de manière contrôlée,
- les comportements limites sont visibles et explicables.

Cette piste d’audit est pensée comme un **outil de validation métier**, au même titre qu’un audit interne ou une revue de modèle.

> Extrait réel de sortie d’audit générée par le moteur QRIS :

```text
=========================================
 FIXED LEG – FOLLOWING, ACT_365F
=========================================

Contractual parameters
----------------------
Start date              : 2025-11-30
End date                : 2026-01-31
Accrual period          : 1M
Stub rule               : ShortBack
Business day convention : Following
Payment lag (days)      : 0
Calendar                : TARGET
Notional                : 1e+06
Currency                : EUR
Pay / Receive           : Receive

#   AccrualStart  AccrualEnd    Days    YearFrac    PaymentDate
----------------------------------------------------------------
1   2025-11-30    2025-12-30    30      0.082192    2025-12-30
2   2025-12-30    2026-01-30    31      0.084932    2026-01-30
3   2026-01-30    2026-01-31    1       0.002740    2026-02-02

Structural checks
-----------------
Accrual periods contiguous        : YES
Payment dates strictly increasing : YES
Coupon shared across cashflows    : YES

Audit conclusion
----------------
STATUS : PASS — leg is contractually coherent
```

---

## 2. Structure de la sortie d’audit

L’audit produit une sortie structurée en quatre parties :

1. **En-tête contractuel**
2. **Paramètres contractuels**
3. **Cashflow schedule détaillé**
4. **Contrôles structurels et conclusion**

Chaque section correspond à un niveau précis de responsabilité du moteur.

---

## 3. Paramètres contractuels audités

La section *Contractual parameters* expose **l’intégralité des hypothèses d’entrée** utilisées pour construire le leg.

### Informations auditées

- **Start date / End date**  
  Intervalle contractuel exact du leg.

- **Accrual period**  
  Périodicité contractuelle abstraite (ex. `1M`), indépendante de toute date réelle.

- **Stub rule**  
  Règle de gestion des périodes incomplètes (ici `ShortBack`).

- **Business Day Convention**  
  Convention d’ajustement des dates non ouvrées (`Following` ou `ModifiedFollowing`).

- **Payment lag**  
  Décalage en jours ouvrés entre la date de fin d’accrual et la date de paiement.

- **Calendar**  
  Calendrier de référence (ici `TARGET`), incluant jours fériés explicites.

- **Notional / Currency / Pay-Receive**  
  Paramètres financiers contractuels invariants.

👉 Cette section garantit que **l’audit est reproductible** et qu’aucune hypothèse implicite n’existe.

---

## 4. Cashflow schedule : lecture détaillée

La table *Cashflow schedule* présente **chaque cashflow individuellement**, avec les informations strictement nécessaires à une analyse contractuelle.

### Colonnes affichées

- **AccrualStart / AccrualEnd**  
  Période exacte sur laquelle les intérêts sont accumulés.

- **Days**  
  Nombre de jours civils entre les deux dates, dérivé de la classe `Date`.

- **YearFrac**  
  Fraction d’année calculée via le module `DayCountConvention`, permettant de :
  - vérifier l’application correcte de ACT/360, ACT/365F, etc.
  - comparer visuellement l’impact des conventions.

- **PaymentDate**  
  Date finale de paiement après :
  1. fin d’accrual,
  2. application du payment lag,
  3. ajustement via la Business Day Convention.

Cette table constitue le **cœur de l’audit**, car elle rend visibles les effets combinés :
- du calendrier,
- des conventions,
- de l’arithmétique des dates.

---

## 5. Illustration des conventions : Following vs Modified Following

L’audit compare volontairement deux legs identiques, ne différant que par :

- la **Business Day Convention**,
- la **Day Count Convention**.

### Cas FOLLOWING

Dans le scénario `Following` :

- une date non ouvrée est **toujours repoussée vers l’avant**,
- même si cela entraîne un changement de mois.

Résultat observé :
- les `PaymentDate` sont strictement croissantes,
- les invariants structurels sont respectés,
- le leg est **contractuellement valide**.

👉 **Audit : PASS**

```text
=========================================
 FIXED LEG – FOLLOWING, ACT_365F
=========================================

Contractual parameters
----------------------
Start date              : 2025-11-30
End date                : 2026-01-31
Accrual period          : 1M
Stub rule               : ShortBack
Business day convention : Following
Payment lag (days)      : 0
Calendar                : TARGET
Notional                : 1e+06
Currency                : EUR
Pay / Receive           : Receive

#   AccrualStart  AccrualEnd    Days    YearFrac    PaymentDate
----------------------------------------------------------------
1   2025-11-30    2025-12-30    30      0.082192    2025-12-30
2   2025-12-30    2026-01-30    31      0.084932    2026-01-30
3   2026-01-30    2026-01-31    1       0.002740    2026-02-02

Structural checks
-----------------
Accrual periods contiguous        : YES
Payment dates strictly increasing : YES
Coupon shared across cashflows    : YES

Audit conclusion
----------------
STATUS : PASS — leg is contractually coherent
```

---

### Cas MODIFIED FOLLOWING

Dans le scénario `ModifiedFollowing` :

- la date est d’abord repoussée vers l’avant,
- **mais** si ce déplacement change de mois, elle est ramenée en arrière.

Résultat observé :
- un paiement peut retomber **avant** un paiement précédent,
- l’ordre strict des dates de paiement peut être violé,
- malgré des accruals parfaitement contigus.

👉 **Audit : FAIL**

```text 
=========================================
 FIXED LEG – MODIFIED FOLLOWING, ACT_360
=========================================

Contractual parameters
----------------------
Start date              : 2025-11-30
End date                : 2026-01-31
Accrual period          : 1M
Stub rule               : ShortBack
Business day convention : ModifiedFollowing
Payment lag (days)      : 0
Calendar                : TARGET
Notional                : 1000000.000000
Currency                : EUR
Pay / Receive           : Receive

#   AccrualStart  AccrualEnd    Days    YearFrac    PaymentDate
----------------------------------------------------------------
1   2025-11-30    2025-12-30    30      0.083333    2025-12-30
2   2025-12-30    2026-01-30    31      0.086111    2026-01-30
3   2026-01-30    2026-01-31    1       0.002778    2026-01-30

Structural checks
-----------------
Accrual periods contiguous        : YES
Payment dates strictly increasing : NO
Coupon shared across cashflows    : YES

Audit conclusion
----------------
STATUS : FAIL — see structural checks above

```

Ce comportement est **attendu**, **réaliste**, et **fondamental à détecter**.

---

## 6. Contrôles structurels

La section *Structural checks* valide trois invariants globaux du leg :

1. **Accrual periods contiguous**  
   Vérifie l’absence de trous ou chevauchements entre périodes.

2. **Payment dates strictly increasing**  
   Garantit la cohérence temporelle globale des flux.

3. **Coupon shared across cashflows**  
   Assure l’unicité contractuelle du coupon sur le leg.

Ces contrôles sont **orthogonaux au pricing** mais critiques pour :
- la robustesse du moteur,
- la génération de flux exploitables,
- la prévention d’erreurs silencieuses.

---

## 7. Conclusion d’audit

L’audit se conclut par un statut explicite :

- **PASS** : le leg est contractuellement cohérent,
- **FAIL** : une incohérence structurelle a été détectée.

Un `FAIL` n’indique **pas un bug**, mais :
- soit un cas contractuellement invalide,
- soit un scénario limite nécessitant une règle métier supplémentaire
  (ex. interdiction de `ModifiedFollowing` sans payment lag).

--- 

## 8. Conclusion générale

Cette sortie d’audit démontre que :

- les briques `Date`, `Calendar`, `BusinessDayConvention`, `DayCountConvention` et `Leg` sont correctement articulées,
- les comportements attendus du marché sont fidèlement reproduits,
- les incohérences ne sont ni masquées ni ignorées, mais **explicitement exposées**.

