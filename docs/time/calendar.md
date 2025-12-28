# Calendar — Gestion des jours ouvrés

## 1. Rôle et positionnement du module

Le module **Calendar** a pour responsabilité exclusive de **déterminer si une date civile donnée est un jour ouvré** dans un contexte de marché donné.

Il constitue une **brique fondamentale du socle temporel** du moteur de taux et intervient **en amont de toute logique financière**, qu’il s’agisse de conventions de marché, de calculs de fractions d’année ou de pricing.

Le calendrier est conçu comme un **objet métier immuable**, purement déclaratif, qui **observe des dates sans jamais les modifier**.

---

## 2. Définition métier d’un jour ouvré

Dans le cadre de ce projet, un **jour ouvré** est défini comme une date civile qui :

* n’est **ni un samedi ni un dimanche**,
* n’est **pas un jour férié de marché**,
* n’est **pas un jour de fermeture officielle** de la place financière considérée.

Cette définition est volontairement **restrictive et explicite**, afin d’éviter toute ambiguïté métier.

---

## 3. Périmètre fonctionnel (v0.1)

### Scope initial

Pour la version **v0.1**, le module Calendar couvre :

* la **zone euro**,
* les **jours ouvrés de bourse**,
* avec les règles suivantes :

  * samedi et dimanche considérés comme non ouvrés,
  * une liste de jours fériés **explicitement fournie et figée à la construction**.

Les jours fériés sont modélisés comme **des dates absolues**, sans règle de calcul dynamique.

---

## 4. Nature du module Calendar

### 4.1 Objet métier immuable

Un `Calendar` :

* est **immuable après construction**,
* ne peut pas être modifié dynamiquement,
* garantit un comportement **déterministe et reproductible**.

Toute modification de règles (changement de zone, ajout ou retrait de jours fériés) implique la création d’une **nouvelle instance**.

Ce choix est fondamental pour :

* la sûreté du calcul,
* la reproductibilité des résultats,
* la simplicité des tests unitaires.

---

### 4.2 Instances paramétrées plutôt qu’héritage

Le projet adopte le principe suivant :

> **Un calendrier est une instance paramétrée, pas une hiérarchie de classes.**

Il n’existe **qu’un seul type `Calendar`**, capable de représenter différents marchés via ses données internes.

Exemples conceptuels :

* `Calendar("TARGET")`
* `Calendar("EURONEXT")`
* `Calendar("NYSE")` *(extension future)*

Ce choix permet :

* d’éviter une hiérarchie rigide et peu extensible,
* de faciliter le chargement dynamique de données,
* de simplifier les tests unitaires,
* de limiter le couplage structurel.

---

## 5. Responsabilités du module

Le module Calendar est responsable de :

* déterminer si une date est ouvrée,
* encapsuler les règles de non-ouvrabilité (week-end, jours fériés),
* exposer l’identité du calendrier (nom ou code de marché).

### Fonction publique minimale

L’API publique est volontairement réduite à l’essentiel :

```
isBusinessDay(Date) → bool
```

Cette fonction répond de manière **déterministe** à la question :

> *« Cette date est-elle un jour ouvré selon ce calendrier ? »*

---

## 6. Frontières strictes (ce que Calendar ne fait pas)

Le module Calendar **ne fait volontairement pas** les opérations suivantes :

* ne modifie jamais une date,
* ne déplace pas une date non ouvrée,
* ne connaît aucune convention de marché,
* ne calcule aucune fraction d’année,
* ne dépend ni du pricing ni des instruments financiers.

Ces frontières sont **structurelles** et garantissent la cohérence globale de l’architecture.

---

## 7. Interaction avec les autres modules temporels

### 7.1 Business Day Convention

L’ajustement d’une date non ouvrée est **exclusivement** de la responsabilité du module **Business Day Convention**.

Relation conceptuelle :

* Calendar : *observe*
* Business Day Convention : *transforme*

Signature conceptuelle attendue :

```
(Date, Calendar) → Date
```

Le Calendar reste totalement **ignorant** de la convention appliquée.

---

### 7.2 Day Count Convention

Le Calendar n’intervient **en aucun cas** dans le calcul des fractions d’année.

Les Day Count Conventions opèrent uniquement sur des **dates déjà déterminées**, indépendamment des règles d’ouvrabilité.

---

## 8. Extensibilité et évolutions futures

### 8.1 Chargement externe des données

À terme, les données de calendrier pourront être :

* chargées depuis des fichiers (CSV, JSON),
* chargées depuis une base de données.

Cette responsabilité sera portée par un composant **externe** (factory ou loader), le `Calendar` restant un **objet métier pur**.

---

### 8.2 Extension à d’autres marchés

Le même type `Calendar` pourra représenter :

* différents pays,
* différentes places financières,
* différents ensembles de jours fériés,

sans modification de l’API publique.

---

## 9. Testabilité

Le module Calendar est conçu pour être :

* **testable indépendamment** de tout autre composant,
* entièrement déterministe,
* sans dépendance au temps système,
* sans dépendance aux données de marché.

Les tests unitaires doivent couvrir au minimum :

* jours de semaine ouvrés,
* week-ends,
* jours fériés,
* dates limites (début / fin d’année).

---

## 10. Hors scope volontaire

Les fonctionnalités suivantes sont explicitement hors du périmètre du module Calendar :

* déplacement de dates,
* conventions de marché,
* calculs financiers,
* gestion des flux,
* pricing.

Ces fonctionnalités seront traitées dans des modules dédiés afin de préserver la **clarté, la robustesse et l’évolutivité** de l’architecture.

---

## 11. Conclusion

Le module Calendar constitue une **brique temporelle fondamentale**, volontairement simple, immuable et strictement délimitée.

Ce design garantit :

* une séparation claire des responsabilités,
* une architecture évolutive,
* une forte testabilité,
* une intégration naturelle avec les conventions et le pricing.

