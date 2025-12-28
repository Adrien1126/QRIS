# PLAN DE DÉVELOPPEMENT — MOTEUR DE TAUX

*(focus Phase 1 → Phase 3, socle solide)*

---

## PHASE 1 — TEMPS & CONVENTIONS

> **Fondation critique**. Rien de financier ne doit être codé avant.

---

### Module 1.1 — `Date` (temps civil)

**Objectif métier**
Représenter une date civile valide, indépendante de toute logique financière.

**Responsabilités**

* représenter une date (Y/M/D),
* comparaison entre dates,
* opérations simples (ajout de jours),
* validation de cohérence (date invalide interdite).

**Interdictions**

* aucune notion de calendrier,
* aucune convention financière,
* aucun calcul de year fraction.

**Critères de validation**

* type immuable après construction,
* opérateurs de comparaison corrects,
* gestion explicite des dates invalides,
* tests unitaires exhaustifs.

👉 **Ce module ne dépend de rien d’autre.**

---

### Module 1.2 — Calendrier

**Objectif métier**
Déterminer si une date est ouvrée dans un contexte donné.

**Responsabilités**

* répondre à : *“cette date est-elle ouvrée ?”*
* encapsuler la logique de jours non ouvrés.

**Scope v0.1**

* zone euro,
* week-end (samedi / dimanche),
* extensible vers jours fériés.

**Interdictions**

* ne modifie jamais une date,
* ne contient aucune logique de déplacement.

**Critères de validation**

* comportement déterministe,
* extensibilité explicite,
* tests sur jours ouvrés / non ouvrés.

---

### Module 1.3 — Business Day Convention

*(Modified Following)*

**Objectif métier**
Ajuster une date non ouvrée selon une règle de marché.

**Responsabilités**

* transformer une date en date ouvrée valide,
* respecter la règle Modified Following.

**Signature conceptuelle**

* entrée : (Date, Calendrier)
* sortie : Date

**Interdictions**

* aucun état interne,
* aucune dépendance au pricing,
* aucune connaissance des instruments.

**Critères de validation**

* date tombant un week-end,
* date qui reste dans le même mois,
* date qui bascule au mois précédent.

---

### Module 1.4 — Day Count Convention

*(ACT/360)*

**Objectif métier**
Convertir deux dates en fraction d’année selon une convention financière.

**Responsabilités**

* calcul déterministe de year fraction,
* respect strict de la convention ACT/360.

**Interdictions**

* ne dépend pas du calendrier,
* ne dépend pas du pricing,
* ne modifie pas les dates.

**Critères de validation**

* cohérence avec formules de référence,
* tests sur périodes simples,
* précision numérique maîtrisée.

---

## PHASE 2 — STRUCTURE CONTRACTUELLE

> **Description du contrat, sans marché, sans pricing.**

---

### Module 2.1 — Cashflow (concept contractuel)

**Objectif métier**
Représenter un paiement contractuel.

**Responsabilités**

* date de paiement,
* notion de montant *potentiel*.

**Distinction clé**

* un cashflow flottant **n’est pas valorisé ici**,
* il devient chiffré uniquement au pricing.

**Interdictions**

* aucun accès aux données de marché,
* aucune logique d’actualisation.

**Critères de validation**

* cohérence des dates,
* immutabilité,
* testabilité sans marché.

---

### Module 2.2 — Leg (Fixed / Floating)

**Objectif métier**
Décrire une structure de paiements.

**Responsabilités**

* périodicité,
* conventions,
* index (pour floating leg),
* rattachement à des règles temporelles.

**Interdictions**

* une leg **ne calcule pas** ses cashflows,
* aucune projection,
* aucune actualisation.

**Critères de validation**

* séparation stricte description / calcul,
* cohérence avec conventions,
* tests sans données de marché.

---

### Module 2.3 — Instrument de taux

**Objectif métier**
Décrire un contrat financier complet.

**Responsabilités**

* composition de legs,
* dates de début / fin,
* notionnel,
* conventions contractuelles.

**Interdictions**

* aucune connaissance du marché,
* aucune notion de prix,
* aucun accès au pricer.

**Critères de validation**

* instrument créable sans marché,
* immutabilité,
* cohérence contractuelle.

---

## PHASE 3 — DONNÉES DE MARCHÉ & PRICING

> **Entrée contrôlée dans le calcul financier.**

---

### Module 3.1 — Données de marché

**Objectif métier**
Représenter un snapshot cohérent du marché.

**Responsabilités**

* courbes forward,
* courbes de discount,
* accès en lecture seule.

**Contraintes**

* immuables entre deux mises à jour,
* partageables entre plusieurs pricings,
* testables indépendamment des instruments.

---

### Module 3.2 — Pricing Engine (actualisation)

**Objectif métier**
Calculer la valeur actuelle d’un instrument.

**Responsabilités**

* projection des cashflows flottants,
* actualisation via courbes de discount,
* agrégation de la VAN.

**Contraintes fortes**

* seul point de contact entre :

  * instrument,
  * cashflows,
  * marché.
* aucun effet de bord.

**Critères de validation**

* pricing déterministe,
* cohérence financière,
* changement de courbe sans impact sur les contrats.

---

## PHASE 4 — CALIBRATION (future, préparée)

---

### Module 4.1 — Calibration

**Objectif métier**

* produire des paramètres de modèles stochastiques,
* ou des courbes de taux exploitables.

**Contraintes**

* disjointe du pricing,
* ne connaît pas les instruments,
* ne modifie pas les données de marché existantes.

---

## ORDRE STRICT D’EXÉCUTION

1. Date
2. Calendrier
3. Business Day Convention
4. Day Count Convention
5. Cashflow
6. Leg
7. Instrument
8. Données de marché
9. Pricing Engine
10. Calibration (plus tard)

👉 **Aucun module ne commence si le précédent n’est pas validé et testé.**


