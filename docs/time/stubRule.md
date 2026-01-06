# StubRule — Spécification contractuelle

## 1. Définition canonique

Un **StubRule** décrit la **règle contractuelle de gestion des périodes partielles** (*stubs*) lors de la génération d’un schedule.

Un stub correspond à une **période d’accrual non standard**, apparaissant lorsque la durée totale d’une leg n’est pas un multiple exact de la périodicité définie par `Period`.

> `StubRule` n’exécute aucune logique temporelle :
> il **oriente le sens et la structure de la génération du schedule**.

---

## 2. Nature du Stub

### 2.1 Élément contractuel explicite

Le stub est :

* une **décision contractuelle**,
* explicitement définie dans la leg,
* nécessaire pour interpréter correctement l’échéancier.

Il n’est **ni implicite**, ni déduit automatiquement sans règle.

---

## 3. Typologie des règles de stub

Le moteur supporte **exactement quatre règles**, correspondant aux conventions de marché :

| StubRule       | Signification           |
| -------------- | ----------------------- |
| **FrontShort** | Période courte au début |
| **FrontLong**  | Période longue au début |
| **BackShort**  | Période courte à la fin |
| **BackLong**   | Période longue à la fin |

Ces règles déterminent :

* **la position du stub** (début ou fin),
* **la taille relative du stub** (court ou long),
* **le sens de génération des dates** (forward ou backward).

---

## 4. Rôle exact dans la génération du schedule

### 4.1 Sens de génération

Le `StubRule` détermine :

* si le schedule est généré :

  * **en avant** depuis `startDate`,
  * ou **en arrière** depuis `endDate`.

Il **n’impose pas de dates**, mais impose **un sens de construction**.

---

### 4.2 Interaction avec la périodicité

Le `StubRule` :

* ne dépend pas directement de `Period`,
* n’effectue aucune opération arithmétique,
* influence uniquement l’algorithme de découpage.

---

## 5. Cas sans stub

Si `startDate` et `endDate` tombent exactement sur la grille définie par `Period` :

* **aucun stub n’existe**,
* le schedule est parfaitement régulier,
* le `StubRule` n’introduit aucune période spéciale.

---

## 6. Cas de durée courte

Si la durée totale de la leg est **strictement inférieure à `Period`** :

* un **unique cashflow** est généré,
* ce cashflow correspond à un stub (court),
* la leg reste **contractuellement valide**.

Aucun rejet n’est effectué.

---

## 7. Responsabilités exclues

Un `StubRule` :

* ❌ ne génère aucune date,
* ❌ ne manipule aucune `Date`,
* ❌ ne connaît aucun `Calendar`,
* ❌ n’ajuste aucun cashflow,
* ❌ ne dépend d’aucune donnée de marché.

Il est **purement déclaratif**.

---

## 8. Validation et invariants

Un `StubRule` est valide si et seulement si :

* il appartient à l’un des **quatre types supportés**,
* il est cohérent avec le sens de génération choisi.

Toute incohérence est :

* détectée **à la construction de la leg**,
* signalée immédiatement par une exception.

---

## 9. Lisibilité et auditabilité

Le `StubRule` :

* doit être **explicitement visible** dans la documentation,
* doit être **loggable / affichable**,
* permet de vérifier rapidement la cohérence d’un échéancier généré.

---

## 10. Positionnement dans le moteur

Le `StubRule` est :

* un **type fondamental du module time**,
* utilisé par le schedule builder,
* indépendant des legs concrètes,
* stable dans le temps.

Il ne s’agit pas d’un simple helper interne.

---

## 3. API conceptuelle cible — `StubRule` (sans code)

Conceptuellement, `StubRule` expose :

* un **ensemble fini de valeurs** (4)
* une **sémantique claire** :

  * position (front / back)
  * taille relative (short / long)

### Opérations conceptuelles autorisées

* construction à partir d’une valeur valide,
* comparaison (égalité),
* exposition de la règle (pour logs / audit).

### Opérations exclues

* génération de dates,
* dépendance à `Period`,
* dépendance à `Date`.




