# Period — Spécification contractuelle

## 1. Définition canonique

Un **Period** représente une **durée temporelle contractuelle abstraite**, exprimée comme une **quantité entière strictement positive associée à une unité de temps**.

Il est utilisé pour décrire :

* des périodicités de paiement,
* des maturités d’instruments,
* des grilles temporelles contractuelles,
* des paramètres de génération de schedules.

> Un `Period` décrit **combien de temps**, jamais **à partir de quand**.

---

## 2. Nature du type

### 2.1 Type descriptif et abstrait

Un `Period` est :

* une **notion descriptive pure**,
* indépendante de toute date concrète,
* indépendante de tout calendrier ou convention de marché.

Un `Period` ne génère **aucune date** et ne possède **aucune logique calendaire**.

---

### 2.2 Unités temporelles supportées

Un `Period` peut être exprimé dans les unités temporelles fondamentales suivantes :

* **Days (D)**
* **Weeks (W)**
* **Months (M)**
* **Years (Y)**

Toute combinaison valide est autorisée, sous réserve de cohérence contractuelle :

* `7D`, `14D`
* `1M`, `3M`, `18M`
* `1Y`, `2Y`, `50Y`

---

## 3. Représentation conceptuelle

### 3.1 Quantité × unité

Conceptuellement, un `Period` est défini par :

* un **multiplicateur entier strictement positif**,
* une **unité temporelle explicite**.

Exemples :

* `3 × Months`
* `1 × Year`
* `14 × Days`

---

### 3.2 Nature des unités temporelles

Les unités temporelles se répartissent en deux catégories conceptuelles :

* **Unités fixes** :

  * Days (D),
  * Weeks (W).
* **Unités calendaires** :

  * Months (M),
  * Years (Y).

Les unités calendaires ne possèdent **aucune conversion universelle vers les jours**.

---

### 3.3 Équivalence contractuelle

Certaines représentations sont **équivalentes contractuellement**, sans être équivalentes calendairement :

* `3M` ≡ `Quarterly`
* `6M` ≡ `SemiAnnual`
* **`12M` ≡ `1Y`**

> La seule équivalence inter-unités autorisée est **12M ≡ 1Y**.

Aucune autre conversion implicite n’est définie :

* `4W ≠ 1M`
* `30D ≠ 1M`
* `365D ≠ 1Y`

---

## 4. Responsabilités exclues

Un `Period` :

* ❌ ne manipule aucune `Date`,
* ❌ ne connaît aucun calendrier,
* ❌ n’applique aucune business day convention,
* ❌ n’effectue aucun calcul financier,
* ❌ ne modifie jamais un objet existant.

Toute opération du type :

> *Date + Period → Date*

relève d’un **composant externe du module `time`**, dédié à l’arithmétique temporelle, et retourne toujours une **nouvelle date**.

---

## 5. Comparaison et ordre

### 5.1 Égalité

Deux `Period` sont égaux si et seulement si :

* ils ont la même unité et le même multiplicateur,
  **ou**
* ils relèvent du cas d’équivalence contractuelle explicitement autorisé
  (**12M ≡ 1Y**, et ses multiples).

Aucune autre égalité inter-unités n’est définie.

---

### 5.2 Ordre

L’ordre entre `Period` est **partiellement défini**.

* Un ordre est défini :

  * à l’intérieur d’une même unité (`3M < 6M`, `1Y < 2Y`),
  * entre mois et années via l’équivalence `12M ≡ 1Y`.
* Certaines périodes sont **incomparables contractuellement** :

  * `15D` et `1M`,
  * `4W` et `1M`.

Ce choix est volontaire et reflète la réalité contractuelle des instruments financiers.

---

## 6. Validation et invariants

Un `Period` est valide si et seulement si :

* le multiplicateur est **strictement positif**,
* l’unité temporelle est **connue et supportée**.

Toute incohérence est :

* détectée **à la construction**,
* signalée immédiatement par une exception.

Aucune instance invalide ne peut exister.

---

## 7. Cas d’usage dans le moteur

Le type `Period` est utilisé pour :

* définir les fréquences de paiement des legs,
* représenter des maturités d’instruments,
* paramétrer la génération de schedules,
* structurer les échéanciers contractuels.

Il constitue une **brique temporelle fondamentale** du moteur.

---

## 8. Extensibilité

Le design de `Period` permet :

* l’ajout futur de conventions spécifiques,
* l’intégration de périodicités exotiques,
* l’extension vers des règles de marché (IMM, etc.),

sans remise en cause de l’API ni des invariants fondamentaux.

---

## 9. API conceptuelle cible (sans code)

Un `Period` expose conceptuellement :

* une **unité temporelle**,
* un **multiplicateur**,
* des **invariants forts de validité**.

### Opérations conceptuelles autorisées

* construction à partir de :

  * multiplicateur + unité,
  * alias métier (Quarterly, SemiAnnual, etc.) ;
* comparaison :

  * égalité contractuelle,
  * ordre partiel ;
* accès en lecture :

  * unité,
  * multiplicateur.

### Opérations explicitement exclues

* ajout à une date,
* ajustement calendaire,
* dépendance au business calendar,
* toute logique de pricing ou de marché.

---

### Conclusion implicite (importante)

`Period` est un **type contractuel fondamental**, volontairement limité,
dont la simplicité et la rigueur garantissent la stabilité de tout le moteur temporel.

