# Leg — Spécification contractuelle

## 1. Définition canonique

Une **Leg** est une **entité contractuelle structurante** représentant une **suite ordonnée de cashflows homogènes**, générés à partir de paramètres contractuels communs (dates, conventions, notionnel, coupon).

Elle constitue le **niveau intermédiaire fondamental** entre :

* les *cashflows* (briques atomiques),
* et les *instruments financiers* (swap, obligation, IRS, etc.).

> Une leg ne valorise rien :
> elle **définit contractuellement comment une série de flux doit être générée**.

---

## 2. Nature de l’objet et principes fondamentaux

### 2.1 Objet contractuel génératif

Contrairement à `Cashflow`, une `Leg` **n’est pas un simple conteneur passif**.

Elle est responsable de :

* la **construction du schedule contractuel**,
* l’**application des conventions calendaires**,
* la **gestion des décalages contractuels** (paiement, fixing),
* la **génération des cashflows** correspondants.

La `Leg` est donc un **objet contractuel génératif**, mais **strictement non calculatoire**.

---

### 2.2 Immuabilité après construction

Une `Leg` est **strictement immuable après construction** :

* l’intégralité des cashflows est générée au constructeur,
* aucun recalcul ou ajustement n’est autorisé ultérieurement,
* toute modification contractuelle implique la création d’une **nouvelle leg**.

Cette immutabilité garantit :

* la stabilité des flux,
* la sûreté des calculs en aval,
* la reproductibilité des audits contractuels.

---

## 3. Frontières strictes : contrat vs calcul

### 3.1 Responsabilités explicitement exclues

Une `Leg` :

* ❌ ne connaît **aucune donnée de marché**,
* ❌ ne connaît **aucune courbe**,
* ❌ ne calcule aucun montant,
* ❌ ne valorise pas les cashflows,
* ❌ ne dépend d’aucun moteur de pricing.

Elle appartient **exclusivement** à la couche *contractuelle*.

---

### 3.2 Responsabilités assumées

Une `Leg` est responsable de :

* la génération des **dates de période**,
* l’application des **business day conventions**,
* la gestion des **stubs**,
* l’application du **payment delay**,
* l’application du **fixing lag** (legs flottantes),
* la création des **cashflows ajustés et finalisés**.

---

## 4. Relation entre Leg et Cashflow

### 4.1 Génération et propriété

Une `Leg` :

* **génère** ses `Cashflow`,
* **possède** ses `Cashflow` par composition,
* expose ensuite une **vue constante** sur ceux-ci.

Les cashflows :

* sont générés **une seule fois**,
* sont **ordonnés chronologiquement**,
* ne sont **jamais recalculés**.

---

### 4.2 Ajustement des dates

Toutes les dates contenues dans les `Cashflow` :

* sont **déjà ajustées** par la `Leg`,
* respectent le calendrier et la business day convention,
* ne tombent jamais sur des jours non ouvrés.

Le `Cashflow` ne réalise **aucun ajustement de dates**.

---

## 5. Typologie des Legs

### 5.1 Legs supportées en v0.1

En première implémentation, le moteur supporte :

* **FixedLeg**
* **FloatingLeg**

Le design permet l’ajout futur de :

* OIS legs,
* Inflation legs,
* CMS legs,
* amortizing legs,
* cross-currency legs.

---

### 5.2 Homogénéité contractuelle

Une `Leg` est **contractuellement homogène** :

* même type de coupon,
* même devise,
* même direction économique,
* mêmes conventions temporelles.

Les structures hétérogènes sont modélisées **au niveau instrument**, par composition de plusieurs legs.

---

## 6. Temps, schedule et conventions

### 6.1 Paramètres temporels portés par la Leg

Une `Leg` est définie par les paramètres contractuels suivants :

* **start date**
* **end date**
* **fréquence de paiement**
* **fréquence de fixing** (le cas échéant)
* **calendar**
* **business day convention**
* **règle de stub** (front / back, short / long)
* **payment delay**
* **fixing lag** (legs flottantes)

Ces paramètres définissent **l’intégralité du schedule contractuel**.

---

### 6.2 Payment Delay

Le **payment delay** représente le décalage contractuel entre :

* la **fin de la période d’accrual**,
* et la **date effective de paiement**.

Caractéristiques :

* exprimé en nombre de jours ouvrés,
* appliqué **après** la génération des dates de période,
* ajusté selon le calendrier et la business day convention.

La `Leg` est responsable de :

> `paymentDate = adjust(accrualEndDate + paymentDelay)`

---

### 6.3 Fixing Lag

Le **fixing lag** représente le décalage contractuel entre :

* le **début de la période d’accrual**,
* et la **date de fixing du taux de référence**.

Caractéristiques :

* exprimé en nombre de jours ouvrés,
* applicable uniquement aux **legs flottantes**,
* calculé à partir de la date d’accrual start,
* ajusté selon le calendrier et la business day convention.

La `Leg` est responsable de :

> `fixingDate = adjust(accrualStartDate − fixingLag)`

---

## 7. Relation avec Coupon

### 7.1 Coupon au niveau de la Leg

Une `Leg` porte :

* un **prototype de coupon**,
* utilisé pour générer les coupons associés à chaque cashflow.

Chaque `Cashflow` possède ensuite **son propre coupon**, immuable.

---

### 7.2 Day Count Convention

La **Day Count Convention appartient au coupon**, et donc indirectement au cashflow.

Justification :

* le day count est une **règle locale au flux**,
* une même leg peut, à terme, contenir des cashflows soumis à des conventions différentes.

---

## 8. Paramètres financiers communs

### 8.1 Notionnel

* Le notionnel est **commun à tous les cashflows** de la leg.
* Les legs amortissantes constituent une **extension future**.

---

### 8.2 Devise

* Tous les cashflows d’une leg sont dans la **même devise**.
* Les produits multi-devises sont modélisés par **plusieurs legs distinctes**.

---

### 8.3 Direction

* La direction (*Pay / Receive*) est **commune à tous les cashflows**.
* Une inversion de signe implique une **leg distincte**.

---

## 9. API publique (vue conceptuelle)

Une `Leg` expose :

### 9.1 Accès aux cashflows

* une collection ordonnée de `Cashflow`,
* exposée en **lecture seule**,
* stable dans le temps.

Une implémentation par `std::vector<Cashflow>` est adaptée.

---

### 9.2 Absence volontaire de calcul

Il n’existe **aucune méthode** du type :

* `amount()`
* `npv()`
* `value()`

Ces responsabilités relèvent exclusivement du **pricing engine**.

---

## 10. Invariants contractuels

Une `Leg` garantit, dès sa construction :

* des dates de début et de fin cohérentes,
* l’existence d’au moins un cashflow,
* un notionnel strictement positif,
* des cashflows ordonnés et valides,
* des dates ajustées conformément aux conventions,
* la cohérence des `paymentDelay` et `fixingLag`.

Toute violation entraîne une **exception immédiate**.

---

## 11. Rôle de la Leg dans le moteur

La `Leg` :

* structure les instruments financiers,
* centralise la logique contractuelle temporelle,
* fournit une entrée stable au pricing,
* supporte l’audit et le reporting,
* prépare les extensions futures (sensibilités, XVA).

---

## 12. Synthèse des décisions structurantes

* `Leg` = **entité contractuelle générative**
* Immuable après construction
* Génère et possède ses `Cashflow`
* Gère schedule, stubs, conventions, delays
* Homogène par construction
* Sans logique de pricing
* Élément structurant des instruments


