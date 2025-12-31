# Cashflow — API cible (contrat métier)

## 1. Définition canonique

Un **cashflow** est un **flux contractuel élémentaire**, représentant une **promesse de paiement future** associée à un instrument financier, **sans aucune logique de valorisation**.

Il constitue la **brique atomique contractuelle** à partir de laquelle sont construites :

* les legs,
* puis les instruments financiers,
* puis les opérations de pricing.

> Un cashflow décrit **ce qui doit être payé, quand, et selon quelles règles contractuelles**, mais jamais combien cela vaut aujourd’hui.

---

## 2. Nature et invariants fondamentaux

### 2.1 Objet métier immuable

Un `Cashflow` est :

* **immuable après construction**,
* figé contractuellement,
* entièrement déterministe.

Toute variation contractuelle (date, notionnel, coupon, signe) implique la création d’un **nouvel objet**.

---

### 2.2 Brique atomique non autonome

Un cashflow :

* **n’a pas de sens économique seul**,
* existe uniquement comme composant d’une :

  * leg,
  * obligation,
  * swap,
  * instrument composite.

Il peut cependant être **construit et inspecté indépendamment** à des fins de test, d’audit ou de validation contractuelle.

---

## 3. Frontières strictes (séparation contractuel / calcul)

### 3.1 Ce que Cashflow ne fait PAS

Un cashflow :

* ❌ ne connaît pas les données de marché,
* ❌ ne connaît aucune courbe,
* ❌ ne s’actualise pas,
* ❌ ne calcule aucune valeur présente,
* ❌ ne contient aucun pricer,
* ❌ ne dépend pas d’un instrument,
* ❌ ne déplace pas ses dates.

Toute logique de calcul est **exclusivement** déléguée aux modules de pricing.

---

### 3.2 Ce que Cashflow PEUT connaître

Un cashflow peut contenir **des informations contractuelles**, même si elles sont utilisées plus tard au pricing :

* type de coupon,
* index sous-jacent (le cas échéant),
* convention de day count,
* accrual period.

Ces informations sont **descriptrices**, pas calculatoires.

---

## 4. Informations portées par un Cashflow

### 4.1 Identité temporelle

Un cashflow porte explicitement :

* **date de paiement** (obligatoire),
* **accrual start date** (si applicable),
* **accrual end date** (si applicable),
* **fixing date** (si applicable, cashflow flottant).

Toutes les dates sont :

* déjà **ajustées par les conventions**,
* fournies par la leg lors de la construction.

---

### 4.2 Identité financière

Un cashflow porte :

* une **devise**,
* un **notionnel** (éventuellement amorti),
* une **direction** (payer / receiver),
* une **description de coupon**.

---

### 4.3 Description du coupon

Un cashflow peut être :

* **fixe** :

  * taux connu,
* **flottant** :

  * index (ex: EURIBOR 3M),
  * spread contractuel,
* **optionnel** (extensible) :

  * cap,
  * floor.

Le cashflow **ne calcule jamais le coupon**, il **décrit les règles** permettant de le calculer plus tard.

---

## 5. Relation avec les conventions

### 5.1 Day Count Convention

Contrairement au calendrier et aux business day conventions :

* la **Day Count Convention appartient au cashflow**.

Justification métier :

* une même leg peut contenir des cashflows soumis à des conventions différentes,
* le day count est une **règle de calcul locale au flux**, pas globale à la leg.

Le cashflow expose donc sa convention de day count, **sans jamais effectuer le calcul**.

---

## 6. API publique cible (conceptuelle)

Un `Cashflow` expose en lecture seule :

### 6.1 Accesseurs temporels

* date de paiement,
* accrual start / end,
* fixing date (si existante).

---

### 6.2 Accesseurs contractuels

* devise,
* notionnel,
* direction,
* type de coupon,
* index sous-jacent (si applicable),
* spread (si applicable),
* taux fixe (si applicable),
* day count convention.

---

### 6.3 Aucune méthode de calcul

Il n’existe **aucune méthode** du type :

* `amount()`,
* `value()`,
* `npv()`,
* `yearFraction()`.

Ces méthodes relèvent exclusivement du **pricing engine**.

---

## 7. Rôle du Cashflow dans le moteur

Le cashflow sert à :

* vérifier la cohérence contractuelle d’un instrument,
* auditer les schedules générés par une leg,
* fournir une entrée propre et stable au pricing,
* supporter le reporting contractuel,
* préparer des extensions futures (XVA, sensitivités, export).

Il constitue un **point de vérité contractuelle**.

---

## 8. Extensibilité

Le design du cashflow permet :

* l’ajout de nouveaux types de coupons,
* l’ajout de paramètres optionnels,
* l’intégration future avec une base de données,
* l’export vers des systèmes externes.

Sans modification de l’API fondamentale.

---

## 9. Résumé des décisions clés

* Cashflow = **donnée contractuelle pure**
* Immuable
* Sans pricing
* Porte ses dates et conventions locales
* Utilisable pour audit et construction
* Élément atomique du moteur
