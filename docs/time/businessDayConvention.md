# Business Day Convention — Module d’ajustement des dates ouvrées

## 1. Rôle et positionnement du module

Le module **Business Day Convention** a pour responsabilité exclusive **d’ajuster une date civile selon une convention de marché**, en s’appuyant sur un calendrier donné.

Il constitue la **troisième brique du socle temporel**, après :

1. `Date` (temps civil),
2. `Calendar` (détermination des jours ouvrés).

Ce module **transforme une date**, mais ne crée aucune logique de marché, de cashflow ou de pricing.

---

## 2. Définition métier

Une **Business Day Convention** est une règle contractuelle qui définit :

> *comment ajuster une date non ouvrée afin d’obtenir une date de paiement valide*.

Cette règle dépend :

* de la **date initiale**,
* du **calendrier de référence**,
* de la **convention choisie**.

Elle est **déterministe**, **sans état**, et **sans effet de bord**.

---

## 3. Nature du module

### 3.1 Fonction pure

Le module est implémenté comme une **fonction libre**, sans état interne :

```
(Date, Calendar, BusinessDayConvention) → Date
```

Ce choix garantit :

* l’absence de couplage,
* la testabilité,
* la composabilité avec les autres briques temporelles.

---

### 3.2 Enumération des conventions

Les conventions sont modélisées via un **enum fortement typé**, représentant explicitement les règles supportées.

Le module ne fait **aucune hypothèse implicite** :
toute convention doit être **déclarée explicitement**.

---

## 4. Conventions supportées (v0.1)

### 4.1 Unadjusted

* La date n’est **jamais modifiée**,
* même si elle tombe un jour non ouvré.

Utilisée dans certains contrats spécifiques ou comme convention neutre.

---

### 4.2 Following

* Si la date est ouvrée → elle est conservée,
* sinon → déplacée vers le **premier jour ouvré suivant**,
* sans considération de changement de mois.

---

### 4.3 Modified Following

* Si la date est ouvrée → elle est conservée,
* sinon → déplacée vers le **premier jour ouvré suivant**,
* **sauf** si ce déplacement entraîne un **changement de mois**,
* dans ce cas → déplacement vers le **dernier jour ouvré précédent**.

C’est l’une des conventions les plus utilisées sur les marchés de taux.

---

### 4.4 Preceding

* Si la date est ouvrée → elle est conservée,
* sinon → déplacée vers le **dernier jour ouvré précédent**.

---

### 4.5 Modified Preceding

* Si la date est ouvrée → elle est conservée,
* sinon → déplacée vers le **dernier jour ouvré précédent**,
* **sauf** si ce déplacement entraîne un **changement de mois**,
* dans ce cas → déplacement vers le **premier jour ouvré suivant**.

---

## 5. Responsabilités du module

Le module Business Day Convention est responsable de :

* ajuster une date non ouvrée,
* appliquer strictement la convention choisie,
* respecter les règles du calendrier fourni,
* garantir une date de sortie **ouvrée et valide**.

---

## 6. Ce que le module ne fait pas (frontières strictes)

Le module **ne** :

* ne détermine pas si une date est ouvrée (rôle du `Calendar`),
* ne modifie pas le calendrier,
* ne connaît aucun instrument financier,
* ne calcule aucune fraction d’année,
* ne dépend d’aucune donnée de marché,
* ne fait aucun pricing.

Cette séparation garantit une **architecture claire et évolutive**.

---

## 7. Interaction avec les autres modules

### 7.1 Relation avec Calendar

Le `Calendar` est utilisé **uniquement en lecture**, via :

```
calendar.isBusinessDay(date)
```

Le module Business Day Convention **ne stocke aucune information de calendrier**.

---

### 7.2 Relation avec Date

Les ajustements sont réalisés exclusivement via les opérations civiles fournies par `Date` :

* comparaison,
* ajout / retrait de jours.

Aucune logique civile n’est dupliquée.

---

## 8. Testabilité

Le module est conçu pour être :

* testé indépendamment,
* déterministe,
* reproductible,
* sans dépendance externe.

Les tests unitaires couvrent notamment :

* dates déjà ouvrées,
* week-ends,
* jours fériés,
* cas critiques de fin de mois,
* différences entre conventions simples et modifiées.

---

## 9. Extensibilité

Le design permet :

* l’ajout de nouvelles conventions sans modifier l’API existante,
* l’intégration future de conventions spécifiques à certains marchés,
* l’utilisation dans tout instrument nécessitant des ajustements de dates.

---

## 10. Conclusion

Le module Business Day Convention constitue une **brique temporelle essentielle**, volontairement simple, pure et strictement délimitée.

Il assure :

* une séparation claire entre observation et transformation,
* une robustesse contractuelle,
* une intégration naturelle avec les calendriers et les instruments financiers.


