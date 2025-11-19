# Black-Scholes Module — Design Document

## 1. Introduction

Le module Black-Scholes est le premier composant du moteur QRIS.  
Il constitue la base du système de pricing et sert de modèle pour les modules
futurs (Heston, SABR, Monte Carlo, etc.).

Ce document décrit :

- la portée du module  
- sa structure interne  
- les classes impliquées  
- les responsabilités de chaque composant  
- les interactions entre les couches  
- les choix de design  
- les risques et points d’attention  

Il ne contient **aucune implémentation**, uniquement le design.

---

## 2. Objectifs du module

### 2.1 Fonctionnels
- Pricing Black-Scholes pour options européennes (Call & Put)
- Calcul des principales sensibilités (Greeks)
- Inversion de volatilité implicite (IV solver)
- Intégration simple avec le reste du moteur

### 2.2 Techniques
- Implémentation propre, modulaire et testable
- Séparation stricte modèle / instrument / pricer
- Pas de dépendance à l’API ou aux Market Data
- Design inspiré des moteurs quant institutionnels
- Tests unitaires garantissant la stabilité numérique

### 2.3 Pédagogiques
- Comprendre la structure d’un moteur quant  
- S’initier au design C++ haute performance  
- Préparer l’architecture pour les modèles avancés  

---

## 3. Portée du module Black-Scholes

### 3.1 Inclus dans le module
- Modèle Black-Scholes classique à volatilité constante
- Pricing analytique fermé
- Calcul de Delta et Gamma
- Outils mathématiques nécessaires (normal CDF/PDF)
- Solver de volatilité implicite (root finding)
- Tests unitaires et documentation

### 3.2 Exclusions (versions futures)
- Theta, Vega, Rho (ajout ultérieur)
- Modèle Bachelier
- Modèle Black-Scholes discrétisé
- Traitements avancés des erreurs numériques
- Pricing Monte Carlo
- Calibration de surfaces de volatilité
- Dividendes discrets

---

## 4. Architecture interne du module

Le module est organisé selon la structure standard QRIS :

```
Instruments → Models → Pricing → Utils
```

Chaque couche est indépendante.

### 4.1 Instruments Layer
Contient les objets financiers.

**Classe principale :**
- EuropeanOption

### 4.2 Models Layer
Décrit les paramètres et la dynamique du sous-jacent.

**Classes :**
- ModelParams  
- BlackScholesModel  

### 4.3 Pricing Layer
Effectue le calcul du prix et des greeks.

**Classes :**
- OptionPricer (interface)
- BlackScholesPricer
- Greeks
- BlackScholesIVSolver

### 4.4 Utils Layer
Fournit les outils mathématiques nécessaires.

**Classes :**
- NormalDistribution  
- MathUtils  

---

## 5. Description détaillée des classes

### 5.1 EuropeanOption
**Rôle :** représenter un instrument européen.  
**Attributs principaux :**
- type (Call / Put)
- strike
- maturité (T)

**Responsabilités :**
- fournir un accès propre aux paramètres  
- rester indépendant du modèle  

---

### 5.2 ModelParams
**Rôle :** regrouper les paramètres financiers nécessaires au modèle.

**Composants :**
- spot
- taux d’intérêt
- volatilité
- dividende

**Objectifs :**
- centraliser les données du modèle  
- simplifier les échanges avec les pricers  

---

### 5.3 BlackScholesModel
**Rôle :** encapsuler les formules internes du modèle.

**Responsabilités :**
- stocker les paramètres  
- fournir les éléments intermédiaires (ex. d1, d2)  
- garantir la cohérence des inputs  

---

### 5.4 OptionPricer (interface)
**Rôle :** définir l’interface commune de tous les pricers.  

**Méthodes clés :**
- price(instrument)  
- greeks(instrument)  

---

### 5.5 BlackScholesPricer
**Rôle :** calculer le prix et les greeks d’une option européenne.

**Responsabilités :**
- utiliser le modèle pour obtenir les grandeurs intermédiaires  
- appliquer les formules de pricing  
- renvoyer un objet Greeks  

**Points d’attention :**
- gestion du cas T → 0  
- stabilité numérique des log-ratios  
- validité des paramètres  

---

### 5.6 Greeks
**Rôle :** structurer le résultat des sensitivités.

**Composants :**
- delta
- gamma
- vega (futur)
- theta (futur)
- rho (futur)

---

### 5.7 BlackScholesIVSolver
**Rôle :** résoudre la volatilité implicite.

**Objectifs :**
- méthode générique compatible avec tous les pricers  
- séparation claire entre inversion et pricing  
- éviter la duplication de code

**Points à définir :**
- méthode numérique (Newton, Brent)  
- critères de convergence  
- gestion des erreurs  

---

### 5.8 NormalDistribution
**Rôle :** fournir la PDF et CDF normales.

### 5.9 MathUtils
**Rôle :** regrouper les fonctions mathématiques de base.

---

## 6. Diagrammes conceptuels

### 6.1 Diagramme de dépendances

```
EuropeanOption
        ↓
BlackScholesPricer  →  Greeks
        ↓
BlackScholesModel  →  ModelParams
        ↓
NormalDistribution, MathUtils
```

---

## 7. Choix de design

### 7.1 Séparation stricte modèle / pricer  
Motivation : réutilisabilité, clarté, testabilité.

### 7.2 Aucun accès Market Data dans le moteur C++  
Motivation : pureté du core, flexibilité.

### 7.3 Interface OptionPricer  
Motivation : polymorphisme pour futurs pricers (Heston, SABR…).

### 7.4 Objects immuables autant que possible  
Motivation : sécurité numérique, multi-threading futur.  

---

## 8. Risques & limites

- erreurs numériques pour très petits T  
- sensibilité à la précision des fonctions normales  
- instabilités pour IV solver (strikes éloignés)  
- limites inhérentes au modèle Black-Scholes  
- absence de smile / skew dans cette première version

---

## 9. Tests et validation

Les tests doivent couvrir :

### 9.1 Tests unitaires
- pricing BS vs valeurs de référence  
- delta / gamma vs dérivées numériques  
- tests sur inputs extrêmes  

### 9.2 Tests d’intégration
- intégration API
