# Floating Leg - Contractual Audit Documentation

## 1. Objectif de l’audit

Ce document décrit la **piste d’audit contractuelle** mise en place dans le moteur **QRIS** afin de valider la **cohérence temporelle et contractuelle** d’un **Floating Leg**.

L’objectif de cet audit n’est **en aucun cas le pricing** ou l’évaluation financière du produit, mais la **vérification explicite, traçable et reproductible** que :

* les échéanciers sont générés conformément aux spécifications contractuelles,
* les conventions de marché sont appliquées de manière correcte et transparente,
* les invariants structurels du leg sont respectés,
* les comportements limites sont détectés, exposés et interprétables.

Contrairement à un **Fixed Leg**, dont la structure dépend essentiellement du calendrier de la leg et de la convention de paiement, un **Floating Leg** introduit une complexité supplémentaire :
les échéanciers dépendent **simultanément** :

* du **calendrier de la leg** (pour les dates d’accrual et de paiement),
* du **calendrier de l’indice sous-jacent** (pour les dates de fixing),
* du **fixing lag**, exprimé en jours ouvrés selon le calendrier de l’indice.

L’objectif principal de cet audit est donc de **valider la cohérence des dates de fixing**, en particulier leur positionnement strictement antérieur au début des périodes d’accrual, ainsi que leur robustesse face aux jours non ouvrés et aux jours fériés spécifiques au calendrier de l’indice.

À des fins pédagogiques et de validation, certains scénarios d’audit supposent explicitement l’existence de jours fériés artificiels (par exemple le **28/11/2025**) afin de rendre visibles et auditables les mécanismes de décalage liés au fixing.

Cette piste d’audit est conçue comme un **outil de validation métier**, comparable à un audit interne, une revue de modèle ou une analyse contractuelle indépendante.
Elle vise à garantir que le moteur QRIS ne masque aucune hypothèse implicite et expose clairement les conséquences contractuelles de chaque convention appliquée.

> Extrait réel de sortie d’audit générée par le moteur QRIS :

```text
=========================================
 FLOATING LEG — CONTRACTUAL AUDIT REPORT
=========================================

Contractual parameters
----------------------
Start date              : 2025-11-30
End date                : 2026-01-31
Accrual period          : 1M
Stub rule               : ShortBack
Payment BDC             : Following
Payment lag (days)      : 0
Calendar                : TARGET
Notional                : 1e+06
Currency                : EUR
Pay / Receive           : Receive

Floating coupon parameters
--------------------------
Index code               : EURIBOR_3M
Index tenor              : 3M
Index fixing lag (bdays) : 2
Index calendar           : TARGET
Spread                   : 0.0015
Coupon day count         : ACT/360

#   FixingDate    AccrualStart  AccrualEnd    Days    YearFrac    PaymentDate   
----------------------------------------------------------------------------
1   2025-11-26    2025-11-30    2025-12-30    30      0.083333    2025-12-30    
2   2025-12-26    2025-12-30    2026-01-30    31      0.086111    2026-01-30    
3   2026-01-28    2026-01-30    2026-01-31    1       0.002778    2026-02-02    

Structural checks
-----------------
Accrual periods contiguous             : YES
Payment dates strictly increasing      : YES
Fixing strictly before accrual start   : YES
Coupon shared across cashflows         : YES
Index shared across cashflows          : YES

Audit conclusion
----------------
STATUS : PASS — floating leg is contractually coherent

=========================================
```

--- 

## 2. Structure de la sorti d'audit 

Cette section expose l’ensemble des paramètres contractuels portés par la Floating Leg elle-même, indépendamment de l’indice sous-jacent.

Elle inclut notamment :

- l’intervalle contractuel (Start date, End date),

- la périodicité d’accrual,

- la règle de gestion des stubs,

- la convention d’ajustement des dates de paiement,

- le calendrier de la leg,

- le notional, la devise et le sens de paiement.

L’objectif est de garantir que toutes les hypothèses structurelles de la leg sont visibles, explicites et auditables.

## 3. Paramètres contractuels audités

La section *Contractual parameters* expose **l’ensemble des hypothèses contractuelles** utilisées pour construire la Floating Leg, indépendamment de toute donnée de marché observée.

Contrairement à une Fixed Leg, ces paramètres ne suffisent pas à déterminer les flux financiers, mais ils conditionnent **l’architecture temporelle** sur laquelle les fixings vont s’appuyer.

### Paramètres audités

* **Start date / End date**
  Bornes contractuelles exactes du leg.
  Elles déterminent :

  * le début du premier accrual,
  * la fin du dernier accrual,
  * la position des éventuels stubs.

* **Accrual period**
  Périodicité contractuelle abstraite (ici `1M`).
  Elle définit la granularité des périodes d’accumulation des intérêts, indépendamment de toute date réelle.

* **Stub rule**
  Règle de gestion des périodes incomplètes (`ShortBack`).
  Dans le cas présent, elle explique l’apparition d’un **stub final d’un jour**.

* **Payment BDC**
  Convention d’ajustement appliquée **uniquement aux dates de paiement**.
  Contrairement à une Fixed Leg, cette convention :

  * n’affecte ni les accruals,
  * ni les dates de fixing,
  * mais uniquement la date finale de règlement.

* **Payment lag**
  Décalage entre la fin d’accrual et la date de paiement, exprimé en jours ouvrés.

* **Calendar**
  Calendrier de paiement utilisé pour :

  * l’ajustement des dates de paiement,
  * la validation des jours ouvrés.

* **Notional / Currency / Pay-Receive**
  Paramètres financiers invariants sur l’ensemble du leg.

Cette section garantit que **toutes les hypothèses contractuelles sont explicites, traçables et reproductibles**.

## 4. Cashflow schedule : lecture détaillée

La table *Cashflow schedule* constitue le **cœur de l’audit Floating Leg**, car elle rend visibles les interactions entre :

* périodes d’accrual,
* dates de fixing,
* conventions calendaires,
* dates de paiement.

### Colonnes affichées

* **FixingDate**
  Date à laquelle le taux de l’index est observé.
  Elle est calculée comme :

  ```
  FixingDate = AccrualStart − fixingLag (en jours ouvrés, calendrier de l’index)
  ```

  Cette colonne permet de vérifier visuellement que :

  * le fixing est strictement antérieur au début de l’accrual,
  * les jours fériés du calendrier de l’index sont correctement pris en compte.

* **AccrualStart / AccrualEnd**
  Bornes exactes de la période sur laquelle les intérêts sont accumulés.

* **Days**
  Nombre de jours civils entre `AccrualStart` et `AccrualEnd`.

* **YearFrac**
  Fraction d’année calculée à partir de la **Day Count Convention du coupon flottant** (`ACT/360` ici).
  Cette valeur permet de :

  * valider la convention utilisée,
  * détecter immédiatement toute incohérence de calcul temporel.

* **PaymentDate**
  Date de paiement finale après :

  1. fin de la période d’accrual,
  2. application du payment lag,
  3. ajustement via la Payment BDC et le calendrier de la leg.

Cette table permet une **lecture contractuelle complète**, sans recours à un pricing ou à des données de marché.

## 5. Illustration des conventions : Fixing Lag

L'audit compare volontairement deux legs ayant des dates différentes de début et de fin. 

### Cas startDate = 30/11/2025 - endDate = 31/1/2026

Résultats observé : 
- les dates de fixing ont bien lieu deux jours ouvrés avant la date de début d'accrual. 
- la leg est **contractuellement valide**. 

**audit : PASS**

```text
=========================================
 FLOATING LEG — CONTRACTUAL AUDIT REPORT
=========================================

Contractual parameters
----------------------
Start date              : 2025-11-30
End date                : 2026-01-31
Accrual period          : 1M
Stub rule               : ShortBack
Payment BDC             : Following
Payment lag (days)      : 0
Calendar                : TARGET
Notional                : 1e+06
Currency                : EUR
Pay / Receive           : Receive

Floating coupon parameters
--------------------------
Index code               : EURIBOR_3M
Index tenor              : 3M
Index fixing lag (bdays) : 2
Index calendar           : TARGET
Spread                   : 0.0015
Coupon day count         : ACT/360

#   FixingDate    AccrualStart  AccrualEnd    Days    YearFrac    PaymentDate   
----------------------------------------------------------------------------
1   2025-11-26    2025-11-30    2025-12-30    30      0.083333    2025-12-30    
2   2025-12-26    2025-12-30    2026-01-30    31      0.086111    2026-01-30    
3   2026-01-28    2026-01-30    2026-01-31    1       0.002778    2026-02-02    

Structural checks
-----------------
Accrual periods contiguous             : YES
Payment dates strictly increasing      : YES
Fixing strictly before accrual start   : YES
Coupon shared across cashflows         : YES
Index shared across cashflows          : YES

Audit conclusion
----------------
STATUS : PASS — floating leg is contractually coherent

=========================================
```

--- 

### Cas startDate = 01/12/2025 - endDate = 31/1/2026

```text
=========================================
 FLOATING LEG — FIXING CALENDAR STRESS
=========================================

=========================================
 FLOATING LEG — CONTRACTUAL AUDIT REPORT
=========================================

Contractual parameters
----------------------
Start date              : 2025-12-01
End date                : 2026-01-31
Accrual period          : 1M
Stub rule               : ShortBack
Payment BDC             : Following
Payment lag (days)      : 0
Calendar                : TARGET
Notional                : 1000000.000000
Currency                : EUR
Pay / Receive           : Receive

Floating coupon parameters
--------------------------
Index code               : EURIBOR_3M
Index tenor              : 3M
Index fixing lag (bdays) : 2
Index calendar           : TARGET
Spread                   : 0.001500
Coupon day count         : ACT/360

#   FixingDate    AccrualStart  AccrualEnd    Days    YearFrac    PaymentDate   
----------------------------------------------------------------------------
1   2025-11-26    2025-12-01    2026-01-01    31      0.086111    2026-01-01    
2   2025-12-30    2026-01-01    2026-01-31    30      0.083333    2026-02-02    

Structural checks
-----------------
Accrual periods contiguous             : YES
Payment dates strictly increasing      : YES
Fixing strictly before accrual start   : YES
Coupon shared across cashflows         : YES
Index shared across cashflows          : YES

Audit conclusion
----------------
STATUS : PASS — floating leg is contractually coherent

=========================================
```

## 6. Contrôles structurels

La section *Structural checks* valide les **invariants fondamentaux** qu’une Floating Leg doit respecter pour être contractuellement exploitable.

Les contrôles effectués sont les suivants :

1. **Accrual periods contiguous**
   Vérifie l’absence de trous ou de chevauchements entre périodes d’accrual.
   Chaque `AccrualEnd` doit correspondre exactement au `AccrualStart` suivant.

2. **Payment dates strictly increasing**
   Garantit la cohérence temporelle globale des flux de paiement.
   Aucun paiement ne doit intervenir avant un paiement précédent.

3. **Fixing strictly before accrual start**
   Invariant fondamental d’un coupon flottant :
   le taux doit être observé **avant** la période sur laquelle il s’applique.

4. **Coupon shared across cashflows**
   Vérifie que tous les cashflows référencent **le même coupon contractuel**, garantissant l’unicité des paramètres financiers.

5. **Index shared across cashflows**
   Vérifie que l’index sous-jacent est identique pour l’ensemble du leg.

Ces contrôles sont **orthogonaux au pricing**, mais critiques pour :

* la robustesse du moteur,
* la lisibilité contractuelle,
* la prévention d’erreurs silencieuses.

---

## 7. Conclusion d’audit

L’audit Floating Leg se conclut par un statut explicite :

* **PASS**
  La Floating Leg est contractuellement cohérente :

  * les périodes d’accrual sont correctement générées,
  * les dates de fixing respectent le fixing lag et le calendrier de l’index,
  * les dates de paiement sont cohérentes et ordonnées,
  * les invariants structurels sont respectés.

* **FAIL**
  Une incohérence contractuelle a été détectée.
  Un échec n’indique **pas nécessairement un bug**, mais :

  * un cas contractuellement invalide,
  * ou un scénario nécessitant une règle métier supplémentaire.
