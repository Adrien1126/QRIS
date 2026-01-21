# QRIS — Quantitative Rates Infrastructure (C++)

QRIS est un **moteur de taux d’intérêt en C++**, conçu comme un projet
**techniquement rigoureux et orienté métier**, dont l’objectif principal est
de **modéliser correctement la structure contractuelle des instruments de taux**
avant toute logique de pricing ou de données de marché.

Le projet met volontairement l’accent sur :

- la **cohérence temporelle**,
- l’application explicite des **conventions de marché**,
- la **traçabilité contractuelle** via des audits lisibles,
- une **architecture C++ propre, testable et extensible**.

---

## 🎯 Objectif du projet

QRIS vise à démontrer qu’un moteur de taux robuste commence par :

1. **un socle temporel irréprochable** (dates, calendriers, conventions),
2. **des contrats passifs et immuables** (cashflows, legs),
3. **des invariants vérifiés explicitement**,
4. une séparation stricte entre :
   - description contractuelle,
   - calcul financier,
   - données de marché.

Le pricing n’intervient **qu’après** validation complète de la structure
contractuelle.

---

## 📌 État actuel du projet (v0.1)

### Implémenté

#### Socle temporel
- `Date`  
  Temps civil, arithmétique de dates, comparaisons, invariants forts.
- `Calendar`  
  Détermination des jours ouvrés (zone TARGET).
- `BusinessDayConvention`  
  Following, Modified Following, Preceding.
- `DayCountConvention`  
  ACT/360, ACT/365F, 30/360 (extensible).

#### Structure contractuelle
- `Cashflow`  
  Flux contractuel **pur**, immuable, sans pricing.
- `Coupon`
  - Fixed coupon
  - Floating coupon (description contractuelle uniquement)
- `Leg`
  - Fixed Leg pleinement implémentée
  - Génération des échéanciers
  - Application explicite des conventions

#### Audit contractuel
- Génération de rapports d’audit lisibles
- Vérification des invariants structurels
- Détection explicite des incohérences temporelles

---

### Explicitement hors scope à ce stade

- ❌ Données de marché
- ❌ Courbes de taux
- ❌ Discounting / projection
- ❌ Pricing / NPV
- ❌ Calibration

Ces briques sont **prévues**, mais volontairement absentes en v0.1.

---

## 🔍 Piste d’audit contractuelle (élément central)

La cohérence des échéanciers générés par les Fixed Legs est validée via
une **piste d’audit dédiée**, pensée comme un outil de revue métier.

📄 **Audit des Fixed Legs**  
➡️ `docs/audit/audit_fixed_leg.md`

Cet audit montre notamment :

- la génération complète des accrual periods,
- l’impact réel des conventions de marché,
- la comparaison `Following` vs `Modified Following`,
- des cas **PASS** et **FAIL** contractuellement expliqués,
- des invariants vérifiés explicitement.

> Un **FAIL d’audit n’est pas un bug** :  
> c’est un comportement de marché réaliste, détecté et rendu visible.

---

## 🧱 Principes de design

- **Séparation stricte des responsabilités**
  - un contrat ne connaît pas le marché,
  - un cashflow ne se valorise jamais seul,
  - les conventions sont sans état.

- **Immutabilité**
  - dates, cashflows, legs sont figés après construction.

- **Auditabilité**
  - aucun ajustement implicite silencieux,
  - tout comportement limite est exposé.

- **Testabilité**
  - modules déterministes,
  - tests unitaires sans dépendance marché.

---

## 📂 Organisation du dépôt

```text
apps/
  auditFixedLeg.cpp        # Exécutable de génération d’audit

include/
  time/                    # Date, Calendar, Conventions
  contract/                # Cashflow, Coupon, Leg
  audit/                   # API d’audit

src/
  time/                    # Implémentations temporelles
  contract/                # Implémentations contractuelles
  audit/                   # Génération des rapports d’audit

docs/
  audit/                   # Pistes d’audit lisibles
  time/                    # Documentation du socle temporel
  contract/                # Documentation contractuelle
  architecture.md          # Vue d’ensemble

tests/
  test_time/               # Tests du socle temporel
  test_contract/           # Tests contractuels
