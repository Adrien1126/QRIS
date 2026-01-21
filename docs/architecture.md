# Architecture — QRIS (v0.1)

## 1. Objectif du document

Ce document décrit l’**architecture effective (as-built)** du moteur de taux QRIS
à l’issue de la version **v0.1**.

Il formalise :

- les **responsabilités réelles** des composants implémentés,
- les **frontières strictes** entre modules,
- les **dépendances autorisées et interdites**,
- la place centrale de l’**audit contractuel**.

Ce document sert de **référence de design** pour :
- l’évaluation du projet,
- l’extension vers les versions futures,
- la prévention de dérives architecturales.

Il ne décrit **aucun détail d’implémentation C++**.

---

## 2. Portée fonctionnelle réelle (v0.1)

La version actuelle du moteur se limite volontairement à :

### Inclus

- la **gestion du temps civil**
  - dates,
  - calendriers,
  - conventions de jours ouvrés,
  - conventions de day count ;
- la **description contractuelle** des instruments de taux ;
- la **construction des échéanciers** ;
- la **génération de cashflows contractuels** ;
- la **validation de la cohérence temporelle** via une piste d’audit dédiée.

### Explicitement exclus (par design)

- toute donnée de marché ;
- toute courbe de taux (forward ou discount) ;
- tout calcul de valeur actuelle (NPV) ;
- tout modèle de taux ;
- toute calibration.

👉 **Aucun pricing n’est implémenté en v0.1.**

---

## 3. Principe fondateur : contrat avant calcul

QRIS repose sur le principe suivant :

> **Un contrat de taux doit être intégralement cohérent,
> lisible et auditable avant toute tentative de pricing.**

En conséquence :

- la **structure contractuelle** est indépendante du marché ;
- les **incohérences temporelles** sont détectées explicitement ;
- aucun ajustement implicite silencieux n’est toléré.

## 4. Socle temporel

Le socle temporel constitue la **fondation la plus stable du moteur**.

### 5.1 Date

`Date` représente le **temps civil pur**.

Responsabilités :

* validation des dates ;
* comparaisons ;
* arithmétique civile ;
* différence en jours civils.

Exclusions :

* aucun calendrier ;
* aucune convention ;
* aucune logique financière.

---

### 5.2 Calendar

`Calendar` détermine **si une date est ouvrée** pour une place donnée.

Responsabilités :

* identification des jours non ouvrés (week-end, jours fériés) ;
* comportement déterministe et immuable.

Exclusions :

* aucun déplacement de date ;
* aucune convention de marché.

---

### 5.3 Business Day Convention

Les conventions de jours ouvrés sont responsables de :

* l’ajustement des dates non ouvrées
  (`Following`, `ModifiedFollowing`, etc.).

Elles :

* utilisent `Calendar` ;
* produisent une **nouvelle date ajustée** ;
* restent sans état.

---

### 5.4 Day Count Convention

Les conventions de day count convertissent :

```
(Date start, Date end) → fraction d’année
```

Responsabilités :

* calcul déterministe de fractions d’année ;
* application stricte des conventions (ACT/360, ACT/365F, etc.).

Exclusions :

* aucun calendrier ;
* aucun ajustement de date ;
* aucun pricing.

---

## 6. Structure contractuelle

### 6.1 Cashflow

Un `Cashflow` représente un **fait contractuel**.

Il est :

* immuable après construction ;
* indépendant du marché ;
* indépendant du pricing.

Il contient :

* une période d’accrual ;
* une date de paiement ;
* une convention de day count ;
* un coupon descriptif (fixe ou flottant).

👉 Un cashflow **ne connaît jamais sa valeur actualisée**.

---

### 6.2 Coupon

Les coupons décrivent **la règle de calcul future** d’un cashflow.

* `FixedCoupon` : taux contractuel fixe.
* `FloatingCoupon` : index + spread (description uniquement).

Ils :

* ne calculent aucun montant ;
* ne connaissent aucune courbe.

---

### 6.3 Leg

Une `Leg` est une **structure contractuelle de paiements**.

Responsabilités :

* porter les conventions ;
* déclencher la génération d’échéanciers ;
* produire une collection cohérente de cashflows.

Exclusions :

* aucun pricing ;
* aucune donnée de marché.

En v0.1, seule la **Fixed Leg** est pleinement implémentée.

---

## 7. Génération des échéanciers

La génération des échéanciers est un **processus déterministe**, dérivé de :

* dates contractuelles ;
* périodicités ;
* conventions ;
* calendrier.

Le composant responsable (ScheduleBuilder):

* applique les règles de stubs ;
* ajuste les dates ;
* garantit la contiguïté temporelle.

Aucun calcul financier n’intervient à ce stade.

---

## 8. Audit contractuel (composant clé)

L’audit contractuel est un **citoyen de premier rang** de l’architecture.

Responsabilités :

* exposer la structure complète des legs ;
* rendre visibles les effets des conventions ;
* vérifier explicitement les invariants structurels ;
* produire des statuts PASS / FAIL.

L’audit :

* ne modifie jamais les contrats ;
* ne dépend d’aucune donnée de marché ;
* ne masque aucun comportement limite.

📄 Référence :
`docs/audit/audit_fixed_leg.md`

---

## 9. Dépendances interdites (règles strictes)

Les dépendances suivantes sont **interdites par design** :

* un contrat ne dépend jamais du marché ;
* un cashflow ne dépend jamais d’un pricer ;
* une leg ne dépend jamais d’un pricer ;
* l’audit ne dépend jamais du marché ;
* le socle temporel ne dépend de rien d’autre que de lui-même.

Ces règles garantissent :

* testabilité maximale ;
* absence de dépendances circulaires ;
* lisibilité métier.

---

## 10. Positionnement des futures extensions

Cette architecture permet naturellement :

* l’ajout d’une Floating Leg sans modifier l’existant ;
* l’introduction d’un pricer sans toucher aux contrats ;
* l’intégration future des données de marché ;
* l’ajout de modèles et de calibration.

Les composants les plus stables resteront :

* le temps ;
* les contrats ;
* les audits.

Les composants amenés à évoluer :

* pricing ;
* modèles ;
* calibration.

---

## 11. Conclusion

L’architecture v0.1 de QRIS repose sur une séparation stricte entre :

* **temps civil**,
* **conventions**,
* **contrats**,
* **audit**.

Cette séparation garantit :

* une cohérence contractuelle démontrable,
* une base saine pour le pricing futur,
* une architecture réaliste et professionnelle.

Le moteur est volontairement **construit par fondations solides**, et non par empilement de calculs.


