# Currency — Devise contractuelle

## 1. Rôle et positionnement du module

Le module **Currency** représente une **devise contractuelle de règlement**, utilisée pour exprimer et régler les flux financiers des contrats manipulés par le moteur.

Il s’agit d’un **type fondamental transverse**, partagé par l’ensemble des briques du système :

* contrats (cashflows, legs, instruments),
* données de marché,
* moteurs de pricing,
* reporting et audit.

`Currency` constitue une **clé économique** essentielle du moteur, mais ne porte **aucune logique financière active**.

---

## 2. Définition métier

Dans le cadre de ce moteur :

> **Une Currency est la devise dans laquelle les flux contractuels sont exprimés et réglés, identifiée de manière unique par son code ISO.**

Elle représente une **unité de compte contractuelle**, indépendante :

* du temps,
* des conventions,
* des modèles de marché,
* des méthodes de pricing.

---

## 3. Identité et unicité

### 3.1 Code ISO comme identifiant unique

Une devise est identifiée **exclusivement** par son **code ISO 4217** (exemples : `EUR`, `USD`, `JPY`).

Règles fondamentales :

* le code ISO est **non vide**,
* il est **normalisé** (majuscules),
* il est **unique** dans le système,
* deux `Currency` ayant le même code ISO sont **strictement identiques**.

Il n’existe aucune autre dimension d’identification (marché, pays, calendrier) à ce niveau.

---

## 4. Relation avec la base de données

La base de données constitue le **référentiel de validité des devises** :

* elle contient la liste des devises supportées,
* chaque devise est identifiée par son code ISO,
* le nombre de devises est **limité et stable**.

Le moteur ne manipule **jamais de chaînes de caractères brutes** pour représenter une devise :

* les chaînes issues de la base de données sont converties en `Currency`,
* toute devise invalide doit être rejetée **à la frontière du système** (loader, factory).

`Currency` reste un **objet métier pur**, ignorant toute logique de persistance.

---

## 5. Invariants garantis par construction

Une instance de `Currency` garantit les invariants suivants :

* le code ISO est valide et non vide,
* l’objet est **immuable** après construction,
* aucune devise invalide ne peut exister dans le cœur du moteur.

Ces invariants assurent une **robustesse structurelle** et évitent toute propagation d’erreurs silencieuses.

---

## 6. Comparaison et égalité

Deux devises sont comparées **uniquement** sur la base de leur code ISO.

* `Currency("EUR") == Currency("EUR")`
* `Currency("EUR") != Currency("USD")`

Aucun ordre économique ou financier n’est défini entre les devises ; seule l’égalité stricte est requise.

Cette propriété permet l’utilisation de `Currency` comme :

* clé logique,
* paramètre contractuel,
* discriminant de marché.

---

## 7. Représentation et audit

`Currency` peut fournir une **représentation textuelle passive**, strictement limitée à son code ISO.

Objectifs de cette représentation :

* logs,
* affichage des cashflows,
* audit contractuel,
* tests unitaires.

Cette représentation ne constitue **pas un mécanisme de parsing** et ne remet pas en cause l’immuabilité ni les invariants.

---

## 8. Ce que Currency ne fait pas (frontières strictes)

Le module `Currency` **n’implémente volontairement aucune logique financière**.

Il ne connaît pas et ne gère pas :

* les conversions de devises (FX),
* les courbes de taux de change,
* les arrondis monétaires,
* les décimales ou unités mineures,
* les calendriers ou conventions,
* les modèles de marché,
* le pricing.

Toute logique de conversion ou de valorisation multi-devises sera portée par des modules dédiés (Market / FX / Pricing).

---

## 9. Interaction avec les autres modules

### 9.1 Contrats

`Currency` est utilisée par :

* `Cashflow` (devise du paiement),
* `Leg` (devise des flux),
* `Instrument` (devise contractuelle globale ou par leg).

### 9.2 Marché et pricing

Les modules de marché et de pricing consomment `Currency` comme :

* clé d’accès aux données,
* discriminant pour les FX swaps,
* référence pour l’actualisation multi-devises.

`Currency` reste toutefois **totalement passive**.

---

## 10. Évolutions futures anticipées

Le design actuel permet, sans rupture d’API :

* l’extension vers des sous-devises (`CNH` vs `CNY`),
* l’ajout de devises non souveraines (crypto),
* l’intégration avec des modules FX,
* l’utilisation de `Currency` comme pivot d’accès marché.

Ces évolutions sont **anticipées**, mais volontairement **hors scope** de la version actuelle.

---

## 11. Conclusion

Le module `Currency` constitue une **brique économique fondamentale** du moteur :

* simple,
* immuable,
* fortement typée,
* indépendante de toute logique financière.


