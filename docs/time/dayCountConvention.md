# Day Count Convention — Calcul des fractions d’année

## 1. Rôle et positionnement du module

Le module **Day Count Convention** a pour responsabilité exclusive de **convertir deux dates civiles en une fraction d’année**, selon une convention financière explicitement définie.

Il constitue la **quatrième brique du socle temporel** du moteur, après :

1. `Date` — temps civil,
2. `Calendar` — détermination des jours ouvrés,
3. `Business Day Convention` — ajustement des dates non ouvrées.

Ce module est **purement calculatoire**, sans état, sans effet de bord, et totalement indépendant du pricing.

---

## 2. Définition métier

Une **Day Count Convention** définit la manière dont le temps est mesuré entre deux dates dans un contrat financier.

Formellement, le module répond à la question :

> *Quelle est la fraction d’année associée à l’intervalle [start, end] selon une convention donnée ?*

Le résultat est utilisé ultérieurement pour :

* le calcul des intérêts,
* la projection de cashflows,
* l’actualisation financière.

---

## 3. Nature du module

### 3.1 Fonction pure

Le module est implémenté sous la forme d’une **fonction libre**, sans état interne :

```
(Date start, Date end, DayCountConvention) → double
```

Cette fonction est :

* déterministe,
* réentrante,
* indépendante de tout contexte externe.

---

### 3.2 Enumération explicite des conventions

Les conventions de décompte sont modélisées via un **enum fortement typé**, garantissant :

* l’absence d’ambiguïté,
* la lisibilité métier,
* l’extensibilité future.

Aucune convention implicite n’est tolérée.

---

## 4. Conventions supportées (v0.1)

### 4.1 ACT/360

**Définition**

La fraction d’année est calculée comme :

[
\frac{\text{nombre de jours civils entre start et end}}{360}
]

**Usage**

* marchés monétaires,
* instruments de taux court terme.

---

### 4.2 ACT/365F (Fixed)

**Définition**

[
\frac{\text{nombre de jours civils entre start et end}}{365}
]

**Usage**

* marchés obligataires,
* instruments à taux fixe.

---

### 4.3 30E/360 (Eurobond)

**Définition**

Les jours sont ramenés à 30 pour les deux dates :

* ( d_1 = \min(\text{day(start)}, 30) )
* ( d_2 = \min(\text{day(end)}, 30) )

[
\frac{
360(y_2 - y_1) + 30(m_2 - m_1) + (d_2 - d_1)
}{360}
]

**Usage**

* obligations Eurobond,
* marchés européens.

---

### 4.4 30/360 US (Bond Basis)

**Définition**

Cette convention suit la formulation décrite dans *Brigo & Mercurio – Interest Rate Models* :

[
\frac{
\max(30 - d_1, 0)

* \min(d_2, 30)
* 360(y_2 - y_1)
* 30(m_2 - m_1 - 1)
  }{360}
  ]

**Exemple**

Entre le 4 janvier 2000 et le 4 juillet 2000 :

[
\text{year fraction} = 0.5
]

**Usage**

* obligations US,
* instruments legacy,
* marchés nord-américains.

---

## 5. Politique sur l’ordre des dates

Le module autorise les périodes **inversées** :

* si `end > start` → fraction positive,
* si `end < start` → fraction négative,
* si `end == start` → fraction nulle.

Ce choix garantit :

* une symétrie mathématique,
* une compatibilité naturelle avec les algorithmes de pricing,
* l’absence d’exceptions inutiles dans les calculs financiers.

---

## 6. Responsabilités du module

Le module Day Count Convention est responsable de :

* calculer une fraction d’année déterministe,
* appliquer strictement la convention sélectionnée,
* garantir la cohérence numérique du résultat.

---

## 7. Ce que le module ne fait pas (frontières strictes)

Le module **ne** :

* ne modifie jamais les dates,
* ne dépend pas du `Calendar`,
* ne connaît pas les jours ouvrés,
* ne gère aucun ajustement de dates,
* ne dépend d’aucune donnée de marché,
* ne fait aucun pricing.

Cette séparation garantit une architecture modulaire et maintenable.

---

## 8. Interaction avec les autres modules

### 8.1 Relation avec Date

Le module repose exclusivement sur les primitives civiles fournies par `Date`, notamment :

* comparaison de dates,
* calcul du nombre de jours civils (`diff`).

Aucune logique civile n’est dupliquée.

---

### 8.2 Relation avec Business Day Convention

Toute correction de dates non ouvrées doit être effectuée **avant** l’appel au module Day Count Convention.

Le calcul de fraction d’année s’applique **uniquement à des dates déjà ajustées**.

---

## 9. Testabilité

Le module est conçu pour être :

* testé indépendamment,
* totalement déterministe,
* reproductible,
* sans dépendance externe.

Les tests unitaires couvrent notamment :

* périodes simples,
* fractions négatives,
* périodes nulles,
* conventions ACT,
* conventions 30/360 (Eurobond et US),
* exemples issus de la littérature de référence.

---

## 10. Extensibilité

Le design permet :

* l’ajout futur de conventions (ACT/ACT ISDA, BUS/252, etc.),
* l’intégration sans modification de l’API existante,
* une compatibilité avec des instruments de plus en plus complexes.

---

## 11. Conclusion

Le module Day Count Convention constitue une **brique temporelle critique**, volontairement simple, pure et strictement délimitée.

Il assure :

* une séparation claire entre temps civil et logique financière,
* une implémentation fidèle aux standards de marché,
* une base solide pour la construction des cashflows et du pricing.


