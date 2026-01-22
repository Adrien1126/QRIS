# RateIndex — Spécification contractuelle (v0.1)

## 1. Objectif et périmètre

### 1.1 Définition

Un **RateIndex** représente un **indice de taux observable sur le marché**, défini par les institutions financières, et destiné à être utilisé comme **référence contractuelle** pour la construction de produits de taux (legs flottants, swaps, FRA, etc.).

Un `RateIndex` **ne contient aucune donnée de marché dynamique** (taux, courbe, volatilité).
Il décrit **exclusivement les conventions immuables** associées à un indice de taux standard.

---

### 1.2 Rôle dans l’architecture QRIS

Dans QRIS, `RateIndex` est un **type fondamental de domaine**, au même niveau conceptuel que `Currency`.

Il sert à :

* identifier de manière unique un indice de taux,
* exposer ses conventions contractuelles,
* garantir la cohérence entre contrats, audit et marché futur.

Il est utilisé par :

* `FloatingCoupon`,
* `FloatingLeg`,
* et, ultérieurement, par la couche Market / Pricing.

---

## 2. Principes de conception

### 2.1 Objet contractuel pur

Un `RateIndex` est :

* **immuable après construction**,
* **indépendant du marché dynamique**,
* **indépendant de toute base de données à l’exécution**,
* **sans logique de calcul**.

Il représente une **donnée de référence**, figée dans le temps.

---

### 2.2 Identité et unicité

L’identité fonctionnelle d’un `RateIndex` est définie par un **code canonique**.

#### Code canonique

* Format : `NOM_TENOR`
* Exemples :

  * `EURIBOR_3M`
  * `ESTR_OIS`

Le code est :

* **normalisé en majuscules**,
* **considéré comme unique**,
* **clé primaire logique** (future DB).

> Deux instances de `RateIndex` portant le même code représentent **le même indice économique**, même si elles sont instanciées séparément.

---

## 3. Données portées par RateIndex

Un `RateIndex` porte **l’intégralité des conventions contractuelles standard** associées à un indice de taux.

### 3.1 Devise

* Devise dans laquelle l’indice est libellé.
* Exemple : EUR pour EURIBOR, ESTR.

---

### 3.2 Tenor

* Tenor de l’indice (ex : 3M, 6M, O/N).
* Le tenor est une **propriété fondamentale** :

  * il définit la nature économique de l’indice,
  * il conditionne la calibration des courbes,
  * il est indispensable pour les modèles type LMM.

---

### 3.3 Fixing lag standard

* Nombre de **jours ouvrés** entre la fixing date et le début de la période d’accrual.
* Exemple :

  * EURIBOR 3M → T-2 business days.

Ce fixing lag est :

* le **standard de marché**,
* **indicatif**, non contraignant pour un contrat spécifique.

---

### 3.4 Day Count Convention standard

* Convention de calcul de la fraction d’année du taux observé.
* Exemple :

  * EURIBOR → ACT/360.

⚠️ Important :
Le **day count de l’index** est **distinct** du day count du coupon ou du leg.

---

### 3.5 Calendrier de référence (fixing & paiement)

Un `RateIndex` porte **un unique calendrier de référence**, utilisé pour :

* déterminer les jours ouvrés de fixing,
* garantir que les dates de fixing sont valides vis-à-vis de l’indice.

Hypothèse v0.1 :

> Les indices standards sont calibrés sur des instruments standards
> (ex : swaps EURIBOR) utilisant **un même calendrier pour fixing et paiement**.

---

### 3.6 Business Day Convention (BDC)

Le `RateIndex` peut porter une **BDC standard**, correspondant aux conventions utilisées lors de la calibration des instruments de marché associés à l’indice.

Cette BDC :

* est **indicative**,
* ne s’impose pas aux contrats.

---

## 4. Frontières avec les autres objets

### 4.1 Relation avec FloatingCoupon

* Le `FloatingCoupon` :

  * porte le **spread**,
  * porte le **day count du coupon**,
  * référence un `RateIndex`.

* Le `RateIndex` :

  * porte les conventions **du taux observé**,
  * n’impose **aucune** convention au coupon.

👉 Les deux sont **conceptuellement décorrélés**.

---

### 4.2 Relation avec FloatingLeg

Le `FloatingLeg` :

* construit les accrual periods,
* génère les payment dates,
* génère les fixing dates.

Le `FloatingLeg` :

* **peut** utiliser ses propres conventions,
* **n’est pas contraint** par celles du `RateIndex`.

Toute incohérence éventuelle :

* doit être **visible à l’audit**,
* n’est pas corrigée silencieusement.

---

## 5. Gestion des dates de fixing

### 5.1 Calendrier de fixing

Le calendrier porté par `RateIndex` définit :

* les jours où un taux peut être fixé,
* les jours de publication officiels.

Si une fixing date tombe sur un jour non ouvré du calendrier de l’index :

* le système doit **ajuster la date selon le calendrier de l’index**,
* ce cas **ne doit pas survenir** en pratique après ajustement.

---

## 6. Relation avec le marché et la base de données

### 6.1 Base de données (future)

À terme :

* la **base de données est la source de vérité** des RateIndex,
* le code canonique est la clé primaire,
* le moteur ne crée jamais d’index “ad hoc”.

Le chargement depuis la DB sera :

* **externalisé** (factory / loader),
* hors de la responsabilité de `RateIndex`.

---

### 6.2 Courbes et pricing

Un `RateIndex` :

* sera **associé indirectement** à des courbes de projection ou de discounting,
* **ne porte jamais** lui-même ces courbes,
* reste ignorant de toute logique de pricing.

---

## 7. Contraintes C++ et design

* Objet **totalement immuable**
* Aucun setter
* Aucun lazy initialization
* Accès uniquement via getters
* Transmission recommandée :

  * `shared_ptr<const RateIndex>` pour éviter toute déformation

---

## 8. Hors scope explicite (à jamais)

Un `RateIndex` **ne fera jamais** :

* calcul de taux,
* stockage de fixings historiques,
* accès à une courbe forward,
* accès à une courbe de discount,
* gestion de volatilité,
* intégration de modèle stochastique,
* projection de flux.

---

## 9. Conclusion

`RateIndex` est une **brique fondamentale de domaine**, volontairement simple, figée et descriptive.

Cette simplicité est :

* un choix architectural assumé,
* une condition nécessaire pour :

  * l’auditabilité,
  * l’extensibilité,
  * l’introduction ultérieure de multicourbe et de modèles de taux.


