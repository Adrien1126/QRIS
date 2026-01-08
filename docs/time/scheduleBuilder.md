# ScheduleBuilder — Spécification contractuelle

## 1. Définition canonique

Un **ScheduleBuilder** est un **outil contractuel interne** chargé de générer un **échéancier d’accrual** à partir de paramètres temporels abstraits.

Il produit une **suite ordonnée de périodes d’accrual**, couvrant un intervalle donné, **sans aucune logique financière, calendaire ou de paiement**.

> Un `ScheduleBuilder` décrit **comment un intervalle de temps est découpé contractuellement**,
> jamais **comment un flux est payé ou valorisé**.

---

## 2. Nature du composant

### 2.1 Outil utilitaire à cycle de vie court

Un `ScheduleBuilder` est :

* un **objet utilitaire**,
* à **cycle de vie court**,
* instancié et utilisé **localement** (typiquement dans le constructeur d’une `Leg`),
* détruit immédiatement après génération du schedule.

Il ne conserve **aucun état persistant**.

---

### 2.2 Composant interne

Le `ScheduleBuilder` :

* n’appartient **pas à l’API publique**,
* est **strictement interne** à la couche contractuelle,
* constitue un **détail d’implémentation** invisible des utilisateurs du moteur.

---

## 3. Responsabilité exacte

### 3.1 Ce que fait `ScheduleBuilder`

Le `ScheduleBuilder` est responsable de :

* découper un intervalle `[startDate, endDate]` en périodes contractuelles,
* appliquer une **fréquence (`Period`)**,
* appliquer une **règle de stub (`StubRule`)**,
* déterminer le **sens de génération** (forward ou backward),
* produire une **suite ordonnée de périodes d’accrual**.

Chaque période est représentée par :

```
(accrualStartDate, accrualEndDate)
```

---

### 3.2 Ce que `ScheduleBuilder` ne fait jamais

Un `ScheduleBuilder` :

* ❌ ne connaît aucun calendrier,
* ❌ n’applique aucune business day convention,
* ❌ ne calcule aucune date de paiement,
* ❌ ne calcule aucune date de fixing,
* ❌ ne connaît ni coupon, ni notional, ni direction,
* ❌ ne dépend d’aucun type de leg.

Il opère **exclusivement sur le temps contractuel abstrait**.

---

## 4. Entrées contractuelles

Un `ScheduleBuilder` est configuré par les paramètres suivants :

### 4.1 Paramètres obligatoires

* **startDate**
  Date de début contractuelle de la leg.

* **endDate**
  Date de fin contractuelle de la leg.

* **paymentFrequency (`Period`)**
  Fréquence contractuelle de découpage.

* **stubRule (`StubRule`)**
  Règle déterminant la position et la nature du stub.

Ces paramètres définissent **entièrement le contrat temporel** du schedule.

---

### 4.2 Paramètres explicitement exclus

Le `ScheduleBuilder` ne reçoit **aucun** des paramètres suivants :

* calendrier,
* business day convention,
* payment delay,
* fixing lag.

Ces éléments sont **hors de son périmètre** et relèvent de la `Leg`.

---

## 5. Sortie produite

### 5.1 Structure de sortie

Le `ScheduleBuilder` produit :

* un `std::vector<std::pair<Date, Date>>`
* chaque élément représentant :

  ```
  (accrualStartDate, accrualEndDate)
  ```

---

### 5.2 Propriétés garanties

Si un schedule est retourné, il garantit :

* une **suite strictement ordonnée** de périodes d’accrual ;
* aucune période vide ou inversée ;
* une **couverture complète** de l’intervalle `[startDate, endDate]` ;
* la présence d’au moins **une période d’accrual** ;
* une gestion correcte des stubs selon la règle fournie.


---

## 6. Sens de génération et gestion des stubs

### 6.1 Sens de génération

Le sens de génération est **déterminé exclusivement par la `StubRule`** :

* **Front Stub** → génération **backward**
* **Back Stub** → génération **forward**

Ce choix garantit que les périodes régulières respectent la fréquence contractuelle.

---

### 6.2 Gestion des stubs

Les stubs :

* sont **gérés entièrement par le `ScheduleBuilder`**,
* peuvent être :

  * short ou long,
  * positionnés en début ou en fin de schedule,
* sont intégrés directement dans la suite de périodes produites.

Aucune information de stub n’est exposée explicitement :
elle est **implicite dans les périodes générées**.

---

## 7. Cas limites et validation

### 7.1 Cas invalides

Le `ScheduleBuilder` lève une exception si :

* `startDate >= endDate`,
* la fréquence est invalide ou incohérente.

---

### 7.2 Cas valides particuliers

Les cas suivants sont **contractuellement valides** et correctement gérés
par le `ScheduleBuilder` :

* intervalle plus court que la fréquence contractuelle  
  → production d’un schedule **à une seule période** couvrant
  `[startDate, endDate]` ;

* schedule composé uniquement d’un stub (short ou long selon la `StubRule`) ;

* schedule à une seule période, indépendamment de la fréquence.

Le `ScheduleBuilder` **ne produit jamais de schedule vide**.
Toute génération valide couvre intégralement l’intervalle
contractuel `[startDate, endDate]`.


---

## 8. Relation avec la `Leg`

### 8.1 Rôle dans la construction d’une Leg

La `Leg` :

1. configure le `ScheduleBuilder`,
2. récupère les périodes d’accrual,
3. applique :

   * payment delay,
   * fixing lag,
   * ajustements calendaires,
4. instancie les `Cashflow`.

---

### 8.2 Séparation stricte des responsabilités

| Composant         | Responsabilité                       |
| ----------------- | ------------------------------------ |
| `ScheduleBuilder` | Découpage temporel contractuel       |
| `Leg`             | Orchestration contractuelle complète |
| `Cashflow`        | Description atomique du flux         |

---

## 9. Invariants garantis

Si un `ScheduleBuilder` retourne un schedule :

* il est **contractuellement sain**,
* il ne nécessite **aucune validation supplémentaire**,
* il peut être consommé directement par une `Leg`.

---

## 10. Synthèse des décisions structurantes

* `ScheduleBuilder` est un **outil interne**
* Génère uniquement des **périodes d’accrual**
* Ignore totalement :

  * paiement,
  * fixing,
  * calendrier,
  * pricing
* Gère :

  * fréquence,
  * stubs,
  * sens de génération
* Produit une sortie simple, déterministe et exploitable

---

### Conclusion

`ScheduleBuilder` est le **socle temporel abstrait** du moteur contractuel.
Il garantit un découpage du temps **correct, neutre et réutilisable**, sur lequel les `Leg` peuvent construire des flux contractuellement irréprochables.

