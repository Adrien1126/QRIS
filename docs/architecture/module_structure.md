# QRIS — Module Structure

## 1. Introduction

Ce document décrit la structure interne du projet QRIS et la répartition des
modules selon une architecture modulaire inspirée des standards quant/risk IT.
Chaque module a un rôle clair, une responsabilité unique et une interface propre.

QRIS est conçu pour être extensible : de nouveaux modèles, pricers ou
composants peuvent être ajoutés sans modifier l’existant.

---

## 2. Vue d’ensemble de la structure des modules

Le moteur QRIS se compose de **quatre couches principales** :

1. **Instruments Layer**  
2. **Models Layer**  
3. **Pricing Layer**  
4. **Utils Layer**

À cela s’ajoutent des modules externes :
- **API Layer (.NET)**  
- **Research Layer (Python)**  
- **Market Data Layer (SQL)**  
- **Interfaces utilisateur (Excel, Web)**

La présente documentation porte principalement sur la structure interne C++.

---

## 3. Instruments Layer

### Rôle
Le module Instruments décrit les objets financiers fondamentaux.  
Il ne contient *aucun modèle* ni *aucune logique de pricing*.

### Objectifs
- représenter les instruments de manière abstraite  
- fournir les paramètres essentiels (strike, maturité, type, etc.)  
- être indépendant de la méthode de pricing  

### Composants
- **EuropeanOption**
- (Extensions futures : BarrierOption, AsianOption, DigitalOption…)

### Responsabilités
- stocker les attributs de l’instrument  
- fournir un accès simple aux données  
- servir de base aux pricers via une interface cohérente  

---

## 4. Models Layer

### Rôle
Encapsuler les modèles mathématiques utilisés pour décrire la dynamique du sous-jacent.

### Objectifs
- séparer clairement modèle et pricing  
- permettre de plugger différents modèles sous la même interface  
- fournir les grandeurs mathématiques propres à chaque modèle  

### Composants actuels
- **ModelParams** (spot, taux, vol, dividende…)  
- **BlackScholesModel**

### Extensions futures
- HestonModel  
- SABRModel  
- BachelierModel  
- LocalVolModel  

### Responsabilités
- fournir les valeurs intermédiaires nécessaires au pricing  
- regrouper les paramètres du modèle dans une structure unique  
- rester totalement indépendant des instruments  

---

## 5. Pricing Layer

### Rôle
Le cœur du moteur : effectuer le pricing et le calcul des greeks.

### Objectifs
- séparer la logique de pricing de la logique du modèle  
- fournir une interface unique pour toutes les méthodes de pricing  
- permettre d’ajouter facilement de nouvelles méthodes (Closed-form, Monte Carlo…)

### Composants actuels
- **OptionPricer (interface)**  
- **BlackScholesPricer**  
- **Greeks**  
- **BlackScholesIVSolver**  

### Extensions futures
- MonteCarloPricer  
- HestonPricer  
- PDEPricer  
- SABRPricer  
- FFTPricer  

### Responsabilités
- exécuter le calcul du prix  
- récupérer les informations auprès du modèle et de l’instrument  
- produire les sensitivités (Greeks)  
- garantir la stabilité numérique  

---

## 6. Utils Layer

### Rôle
Fourni les outils mathématiques et statistiques nécessaires au moteur.

### Objectifs
- isoler les outils génériques du reste du système  
- éviter la duplication de logique  
- garantir la cohérence numérique à travers le moteur  

### Composants
- **NormalDistribution**  
- **MathUtils**  

### Extensions futures
- RandomNumberGenerator  
- Integration methods (Gauss-Hermite, Simpson…)  
- Matrix utilities  

### Responsabilités
- fournir des fonctions de base utilisées par les modèles et pricers  
- offrir un comportement robuste (gestion des limites, stabilité numérique)  

---

## 7. Interactions entre modules

### 7.1 Diagramme conceptuel

```
EuropeanOption  →  OptionPricer  →  BlackScholesModel  →  Utils
```

Détails :

- **L’instrument** décrit *ce qu’on pricer*  
- **Le modèle** décrit *comment évolue le sous-jacent*  
- **Le pricer** décrit *comment on calcule le prix*  
- **Les utils** fournissent *les outils mathématiques*  

Cette séparation permet :
- des tests unitaires simples  
- une maintenance plus facile  
- un ajout de modèles non intrusif  
- une structure très proche de l'industrie  

---

## 8. Modules externes

### 8.1 API .NET Layer
Rôle :
- exposer les pricers via HTTP  
- gérer les inputs et outputs  
- orchestrer C++ / Market Data / Excel  

Composants clés :
- Controllers  
- PricingService  
- Interop C++  
- Models (DTO)

---

### 8.2 Python Research Layer
Rôle :
- tests exploratoires  
- visualisation de résultats  
- prototypage ML  

Composants :
- notebooks  
- scripts de simulation  
- modèles ML  

---

### 8.3 SQL Data Layer
Rôle :
- stocker les market data  
- fournir les données au moteur via l’API  

Composants :
- tables `spot`, `rates`, `vol`  
- seeds  
- migrations  

---

### 8.4 Excel Add-in
Rôle :
- permettre aux utilisateurs non techniques d’utiliser le moteur  
- fournir des fonctions de pricing simples  

Composants :
- fonctions QRIS.BS  
- helpers Excel  

---

## 9. Extensibilité du système

Le design modulaire permet l’ajout de :

- nouveaux modèles  
- nouvelles méthodes numériques  
- nouveaux types d’instruments  
- nouvelles sources de market data  
- nouveaux endpoints API  
- nouvelles interfaces utilisateur  

Chaque ajout doit respecter :
- la séparation des modules  
- l’indépendance entre couches  
- la documentation  
- les tests unitaires  

---

## 10. Conclusion

La structure modulaire de QRIS garantit une évolution progressive et propre du
projet.  
Elle reproduit l’architecture des moteurs quantitatifs institutionnels tout en
étant adaptée à un développement individuel.

Ce document sert de référence pour comprendre la répartition des modules,
orienter le développement et maintenir la cohérence globale du système.
