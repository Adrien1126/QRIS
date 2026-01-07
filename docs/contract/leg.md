# Leg — Spécification contractuelle

## 1. Définition canonique

Une **Leg** est une **entité contractuelle structurante** représentant une **suite ordonnée de cashflows homogènes**, générés à partir de paramètres contractuels communs (dates, conventions, notionnel, coupon).

Elle constitue le **niveau intermédiaire fondamental** entre :

* les *cashflows* (briques atomiques),
* et les *instruments financiers* (swap, obligation, IRS, etc.).

> Une leg ne valorise rien :
> elle **définit contractuellement comment une série de flux doit être générée**, sans jamais effectuer de calcul de montant.

---

## 2. Nature de l’objet et principes fondamentaux

### 2.1 Objet contractuel génératif

Contrairement à `Cashflow`, une `Leg` **n’est pas un simple conteneur passif**.

Elle est responsable de :

* la **définition des paramètres contractuels temporels**,
* l’**orchestration de la génération du schedule contractuel**,
* l’**application des conventions calendaires et des décalages contractuels**,
* la **génération des cashflows finaux et ajustés**.

La `Leg` est donc un **objet contractuel génératif**, mais **strictement non calculatoire**.

---

### 2.2 Immuabilité après construction

Une `Leg` est **strictement immuable après construction** :

* l’intégralité du schedule est générée une seule fois,
* l’intégralité des cashflows est instanciée au constructeur,
* aucun recalcul ou ajustement n’est autorisé ultérieurement.

Toute modification contractuelle implique la création d’une **nouvelle instance de Leg**.

Cette immutabilité garantit :

* la stabilité contractuelle des flux,
* la sûreté des calculs en aval,
* la reproductibilité des audits.

---

## 3. Frontières strictes : contrat vs calcul

### 3.1 Responsabilités explicitement exclues

Une `Leg` :

* ❌ ne connaît **aucune donnée de marché**,
* ❌ ne connaît **aucune courbe**,
* ❌ ne calcule aucun montant,
* ❌ ne valorise pas les cashflows,
* ❌ ne dépend d’aucun moteur de pricing.

Elle appartient **exclusivement à la couche contractuelle**.

---

### 3.2 Responsabilités assumées

Une `Leg` est responsable de :

* la configuration des règles temporelles contractuelles,
* l’orchestration de la génération de l’échéancier,
* l’application des règles de stub,
* l’application du **payment delay**,
* l’application du **fixing lag** (le cas échéant),
* la création de cashflows **contractuellement finalisés**.

---

## 4. Relation entre Leg, ScheduleBuilder et Cashflow

### 4.1 Séparation des responsabilités

La génération temporelle est scindée en deux niveaux distincts :

* **ScheduleBuilder**

  * génère un échéancier contractuel cohérent,
  * applique les règles de fréquence, de stub et d’ajustement calendaire,
  * ne connaît ni notionnel, ni coupon, ni direction.

* **Leg**

  * configure le `ScheduleBuilder`,
  * consomme l’échéancier généré,
  * transforme chaque période en `Cashflow`.

Cette séparation garantit :

* une responsabilité unique par composant,
* une testabilité fine du module temporel,
* une extensibilité propre.

---

### 4.2 Génération et propriété des Cashflows

Une `Leg` :

* **génère** ses `Cashflow`,
* **possède** ses `Cashflow` par composition,
* expose ensuite une **vue constante** sur ceux-ci.

Les cashflows :

* sont générés **une seule fois**,
* sont **ordonnés chronologiquement**,
* ne sont **jamais recalculés**.

---

### 4.3 Ajustement des dates

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
* **fréquence de paiement** (`Period`)
* **calendar**
* **business day convention**
* **règle de stub** (`StubRule`)
* **payment delay**
* **fixing lag** (legs flottantes uniquement)

Ces paramètres définissent **l’intégralité du schedule contractuel**.

---

### 6.2 Rôle du ScheduleBuilder

La génération de l’échéancier est **déléguée à un composant dédié** (`ScheduleBuilder`).

Ce composant est responsable exclusivement de :

* la génération des dates de période,
* l’application des règles de stub,
* l’ajustement calendaire,
* la cohérence temporelle globale.

La `Leg` **configure et consomme** ce schedule, sans en porter la logique interne.

---

### 6.3 Payment Delay

Le **payment delay** représente le décalage contractuel entre :

* la **fin de la période d’accrual**,
* et la **date effective de paiement**.

Caractéristiques :

* exprimé en nombre de jours ouvrés,
* appliqué après la génération de l’échéancier,
* ajusté selon le calendrier et la business day convention.

---

### 6.4 Fixing Lag

Le **fixing lag** représente le décalage contractuel entre :

* le **début de la période d’accrual**,
* et la **date de fixing du taux de référence**.

Caractéristiques :

* exprimé en nombre de jours ouvrés,
* applicable uniquement aux legs flottantes,
* ajusté selon le calendrier et la business day convention.

---

## 7. Relation avec Coupon

### 7.1 Coupon au niveau de la Leg

Une `Leg` porte un **prototype de coupon**, utilisé pour instancier les coupons associés à chaque cashflow.

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

* une collection ordonnée de `Cashflow`,
* en **lecture seule**,
* stable dans le temps.

Une implémentation par `std::vector<Cashflow>` est appropriée.

Aucune méthode de calcul financier n’est exposée.

---

## 10. Invariants contractuels

Une `Leg` garantit, dès sa construction :

* des dates de début et de fin cohérentes,
* l’existence d’au moins un cashflow,
* un notionnel strictement positif,
* des cashflows ordonnés et valides,
* des dates ajustées conformément aux conventions,
* la cohérence des délais contractuels.

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
* Orchestration via `ScheduleBuilder`
* Génère et possède ses `Cashflow`
* Gère conventions, stubs et délais
* Homogène par construction
* Sans logique de pricing
