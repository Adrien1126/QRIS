# QRIS — Architecture Overview

## 1. Introduction

QRIS (Quantitative Risk & Pricing Engine) est un moteur quantitatif modulaire
conçu pour le pricing d’options, le calcul de risques et l’analyse financière.
Le projet suit les standards des équipes quant/risk IT : séparation claire des
responsabilités, modularité, extensibilité et documentation complète.

Ce document présente la vue d’ensemble du système, ses composants majeurs,
leurs interactions et la philosophie générale du projet.

---

## 2. Objectifs du système

QRIS vise à :

- proposer une architecture claire pour des modèles quantitatifs (BS, Heston…)
- offrir un moteur haute performance pour le pricing (C++)
- permettre une exposition simple via une API moderne (.NET)
- offrir un environnement de recherche flexible (Python)
- fournir des interfaces utilisateurs (Excel, Webapp)
- mettre en place une gestion propre des données de marché (SQL)
- servir de support d'apprentissage en finance quantitative et ingénierie logicielle

---

## 3. Vue d’ensemble de l’architecture

Le système se compose de cinq couches principales :

1. **C++ Core Engine**  
   - Implémentation des modèles quantitatifs  
   - Pricing haute performance  
   - Calcul des sensitivités (Greeks)

2. **Python Research Layer**  
   - Notebook de recherche  
   - Prototypage de modèles  
   - Expérimentation ML  

3. **.NET API Layer**  
   - Interface REST  
   - Orchestration entre moteurs et data  
   - Conversion d’inputs/outputs  
   - Intégration avec Excel et Webapp

4. **SQL Data Layer**  
   - Market data (spot, taux, vol)  
   - Instruments  
   - Surfaces de volatilité  
   - Historique

5. **Front-End Layer**  
   - Add-in Excel  
   - Dashboard Web (optionnel)

---

## 4. Diagramme conceptuel

```
C++ Core       → Pricing haute performance
Python         → Recherche & prototypage
.NET API       → Exposition & orchestration
SQL            → Stockage et market data
Excel / Web    → Interfaces utilisateur
```

L’API sert de couche centrale connectant toutes les autres parties.

---

## 5. Principes d’architecture

QRIS respecte plusieurs principes fondamentaux :

### 5.1 Séparation des responsabilités
- **Instruments** ≠ **Modèles** ≠ **Pricers**
- **API** ≠ **Core**  
- **Market Data** totalement indépendante des modèles

### 5.2 Modularité
Chaque module peut être remplacé sans impacter les autres.

Exemple :  
le pricing Black-Scholes peut être remplacé par Heston sans modifier l’API.

### 5.3 Extensibilité
L’architecture permet d’ajouter facilement :
- nouveaux modèles (SABR, Bates…)  
- nouvelles méthodes numériques (MC, FFT)  
- nouvelles interfaces (CLI, Webapp)

### 5.4 Indépendance technologique
Les composants interagissent par **interfaces**, pas par dépendances directes.

---

## 6. Description des sous-systèmes

### 6.1 C++ Core Engine
- Pricing Black-Scholes  
- Greeks  
- Calcul de volatilité implicite  
- Monte Carlo (futur)  
- Modèles volatiles (Heston, SABR)  

### 6.2 Python Research Layer
- Prototypage rapide  
- Visualisation et notebooks  
- Préparation des modèles ML  
- Analyse top-down  
- Benchmarks  

### 6.3 API .NET Layer
- Endpoints `/pricing/*`  
- Intégration C++ via P/Invoke  
- Intégration Python/ONNX  
- Exposure vers Excel et Webapp  
- Service de Market Data  

### 6.4 SQL Data Layer
- Tables pour spot, taux, dividendes  
- Tables pour vol surfaces  
- Migrations et seeding  
- Stockage long terme  

### 6.5 Interfaces Utilisateur
- Add-in Excel  
- Dashboard web (optionnel)  

---

## 7. Flux de données

1. L’utilisateur (Excel, Webapp) envoie une demande de pricing.
2. L’API valide et normalise les inputs.
3. L’API appelle le moteur C++ via un interop.
4. Le moteur C++ calcule le prix/greeks.
5. Le résultat est renvoyé à l’utilisateur.
6. (Option) Python peut intervenir pour des calculs ML.

---

## 8. Roadmap Architecture

Les prochaines étapes de l’architecture comprennent :

- ajout du PricingContext
- mise en place du MarketDataProvider
- ajout des courbes & surfaces
- introduction du module Monte Carlo
- architecture ML (ONNX Runtime)

---

## 9. Conclusion

QRIS est conçu pour être un moteur quantitatif complet, professionnel et
extensible.  
L’architecture est volontairement inspirée de ce qui se fait dans les salles de
marché, tout en restant adaptée à un développement individuel et progressif.

Les modules pourront évoluer indépendamment tout en respectant la cohérence et
la clarté du système.

