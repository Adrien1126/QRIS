# Cashflow — Spécification contractuelle

## 1. Définition canonique

Un **Cashflow** représente un **flux contractuel élémentaire**, c’est-à-dire une **promesse de paiement future** associée à un instrument financier, **indépendante de toute logique de valorisation**.

Il constitue la **brique atomique contractuelle** du moteur, à partir de laquelle sont construits :

* les *legs*,
* les instruments financiers,
* puis les opérations de pricing, de risque et de reporting.

> Un cashflow décrit **ce qui est dû**, **quand**, **dans quelle devise**, et **selon quelles règles contractuelles**,
> mais **ne dit jamais combien cela vaut aujourd’hui**.

---

## 2. Nature de l’objet et invariants fondamentaux

### 2.1 Objet métier immuable

Un `Cashflow` est un **objet métier strictement immuable** :

* son état est entièrement défini à la construction,
* aucune mutation n’est possible après instanciation,
* toute modification contractuelle implique la création d’un **nouvel objet**.

Cette immutabilité garantit :

* la sûreté des calculs en aval,
* l’absence d’effets de bord,
* la stabilité des audits contractuels.

---

### 2.2 Objet contractuel non autonome

Un cashflow :

* **n’a pas de sens économique isolément**,
* existe exclusivement comme composant d’un ensemble contractuel plus large
  (leg, obligation, swap, instrument composite).

Il peut toutefois être :

* construit indépendamment,
* inspecté via son API,
* testé et validé isolément,

à des fins de **validation contractuelle**, **audit** ou **tests unitaires**.

---

## 3. Frontières strictes : contrat vs calcul

### 3.1 Responsabilités explicitement exclues

Un `Cashflow` :

* ❌ ne connaît **aucune donnée de marché**,
* ❌ ne connaît **aucune courbe** (discount, forward, funding),
* ❌ ne s’actualise pas,
* ❌ ne calcule aucun montant,
* ❌ ne contient aucun pricer,
* ❌ ne dépend d’aucun instrument financier,
* ❌ ne modifie ni n’ajuste ses dates.

Toute logique numérique ou financière est **exclusivement déléguée** aux modules de pricing.

---

### 3.2 Responsabilités autorisées

Un `Cashflow` peut porter des **informations contractuelles descriptives**, nécessaires aux calculs futurs mais **sans jamais les effectuer** :

* type de coupon,
* paramètres du coupon (taux, index, spread),
* convention de day count,
* période d’accrual,
* date de fixing (le cas échéant).

Ces informations sont **descriptrices**, jamais calculatoires.

---

## 4. Informations portées par un Cashflow

### 4.1 Identité temporelle

Un cashflow porte explicitement :

* une **date de paiement** (obligatoire),
* une **date de début d’accrual** (optionnelle),
* une **date de fin d’accrual** (optionnelle),
* une **date de fixing** (optionnelle, cashflows flottants).

Toutes les dates sont supposées :

* déjà ajustées par les conventions calendaires et business day,
* fournies par la *leg* lors de la construction.

Le cashflow **ne réalise aucun ajustement de dates**.

---

### 4.2 Identité financière

Un cashflow porte :

* une **devise**,
* un **notionnel** strictement positif,
* une **direction économique** (*Pay* / *Receive*),
* une **description contractuelle de coupon**.

Le signe économique du flux est **porté par la direction**, jamais par le notionnel.

---

### 4.3 Description du coupon

Le coupon associé à un cashflow décrit les **règles contractuelles de calcul**, sans jamais produire de montant.

Il peut être :

* **fixe** :

  * taux contractuel connu,
* **flottant** :

  * index de référence (ex. *EURIBOR 3M*),
  * spread contractuel,
* **optionnel / extensible** :

  * cap,
  * floor,
  * autres structures futures.

Le cashflow **possède** son coupon et en expose une **référence constante**.

---

## 5. Conventions financières

### 5.1 Day Count Convention

La **Day Count Convention appartient au coupon**, et donc indirectement au cashflow.

Justification métier :

* une même leg peut contenir des cashflows soumis à des conventions différentes,
* le day count est une **règle locale au flux**, pas une règle globale à la leg.

Le cashflow expose cette convention **sans jamais effectuer le calcul de fraction d’année**.

---

## 6. API publique (vue conceptuelle)

Un `Cashflow` expose **uniquement des accesseurs en lecture seule**.

### 6.1 Accesseurs temporels

* date de paiement,
* accrual start date (si existante),
* accrual end date (si existante),
* fixing date (si existante).

---

### 6.2 Accesseurs contractuels

* devise,
* notionnel,
* direction (*Pay / Receive*),
* coupon (via interface polymorphique),
* type de coupon,
* paramètres du coupon (taux, index, spread),
* convention de day count.

---

### 6.3 Absence volontaire de méthodes de calcul

Il n’existe **délibérément aucune méthode** du type :

* `amount()`,
* `value()`,
* `npv()`,
* `yearFraction()`.

Ces méthodes relèvent **exclusivement** du moteur de pricing.

---

## 7. Rôle du Cashflow dans le moteur

Le cashflow constitue un **point de vérité contractuelle**.
Il est utilisé pour :

* vérifier la cohérence contractuelle d’un instrument,
* auditer les échéanciers générés par une leg,
* fournir une entrée propre et stable au pricing,
* supporter le reporting contractuel,
* préparer les extensions futures (sensibilités, XVA, export).

---

## 8. Extensibilité du design

Le design du `Cashflow` permet :

* l’ajout de nouveaux types de coupons,
* l’introduction de paramètres contractuels optionnels,
* une intégration future avec une base de données,
* l’export vers des systèmes externes.

Le tout **sans modification de l’API fondamentale**.

---

## 9. Synthèse des décisions structurantes

* `Cashflow` est une **donnée contractuelle pure**
* objet **immuable**
* **aucune logique de pricing**
* porte ses dates et conventions locales
* utilisable pour audit et construction
* **élément atomique du moteur**


