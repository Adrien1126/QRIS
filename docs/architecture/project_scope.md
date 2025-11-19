# QRIS — Project Scope

## 1. Contexte

QRIS (Quantitative Risk & Pricing Engine) est un projet personnel développé
dans un cadre d’apprentissage et de démonstration professionnelle.  
L’objectif est de construire un moteur quantitatif modulaire similaire à ceux
utilisés en finance de marché, tout en structurant les connaissances
acquises durant un Master en Finance Quantitative.

Ce document définit le périmètre fonctionnel et technique du projet
pour assurer une progression claire, priorisée et professionnelle.

---

## 2. Objectifs du projet

Les objectifs principaux de QRIS sont :

### 2.1 Objectifs techniques
- Construire un moteur de pricing haute performance en C++
- Implémenter des modèles fondamentaux de finance quantitative
- Exposer le moteur via une API moderne en .NET
- Créer une interface utilisateur simple via Excel
- Mettre en place une organisation modulaire et extensible
- Rédiger une documentation professionnelle et pédagogique

### 2.2 Objectifs pédagogiques
- Approfondir la compréhension des modèles de pricing
- Renforcer les compétences C++ (design, architecture, patterns)
- Développer des compétences API / backend (C#, REST)
- Documenter les processus, décisions et limites
- Approcher les standards quant/risk IT des grandes institutions
- Créer un portfolio professionnel valorisable auprès des recruteurs

### 2.3 Objectifs professionnels
- Montrer la capacité à structurer un système quant
- Fournir un exemple concret de design logiciel
- Illustrer la compétence à travailler de manière autonome
- Préparer des démonstrations pour des entretiens techniques

---

## 3. Périmètre fonctionnel (In-Scope)

Les fonctionnalités suivantes font partie intégrante du projet QRIS.

### 3.1 C++ Core Engine
- Pricing Black-Scholes (Call, Put)
- Calcul des Greeks
- Inversion de volatilité implicite
- Architecture pour modèles futurs (Heston, SABR)
- Base pour Monte Carlo
- Module math/statistique (utils)

### 3.2 API .NET
- Endpoints pour pricing et Greeks
- Communication avec la DLL C++
- Modèles d’entrée et sortie standardisés
- Validation des inputs
- Documentation Swagger

### 3.3 Excel Add-in
- Fonction de pricing BS (QRIS.BS)
- Formatage du résultat
- Chargement simple des données

### 3.4 Documentation
- Architecture globale
- Théorie (Black-Scholes, Greeks…)
- Design des modules
- Roadmap technique
- Journal d’apprentissage

### 3.5 SQL (Version minimale)
- Tables pour market data basiques
- Seeds pour spot, taux, volatilité simple
- Migrations initiales

---

## 4. Hors Périmètre (Out-of-Scope)

Pour maintenir le projet clair et réalisable en solo, les éléments suivants sont **hors périmètre initial**.  
Ils pourront être ajoutés plus tard.

### 4.1 Fonctionnalités non prévues pour la première version
- Modèles de volatilité locale (Dupire)
- Volatility surfaces avancées
- Modèle Stochastic Alpha Beta Rho complet (SABR)
- Modèles jump-diffusion (Bates, Merton)
- Portefeuilles multi-instruments
- Gestion du risque globale (VaR, CVA, XVA)
- Infrastructure cloud
- Base de données temps réel (Kafka, Redis)

### 4.2 Hors périmètre côté interfaces
- Interface web complète
- Dashboard avancé type Bloomberg
- Application mobile

### 4.3 Hors périmètre ML (phase ultérieure)
- Deep hedging
- Neural SDE
- Calibration via réseaux neuronaux
- Volatility forecasting via LSTM / transformer

---

## 5. Hypothèses et Contraintes

### 5.1 Hypothèses
- Le moteur doit rester simple et lisible
- Le projet est développé par un seul contributeur
- Le planning est volontairement flexible mais structuré

### 5.2 Contraintes
- Ressources limitées (temps + équipe)
- Environnement technique géré seul
- Documentation obligatoire pour chaque module

---

## 6. Livrables principaux

À la fin du projet QRIS v1.0, les livrables attendus sont :

- **Core C++ Black-Scholes Engine**  
- **API .NET fonctionnelle**  
- **Add-in Excel pour pricing BS**  
- **Documentation complète**  
- **Tests unitaires C++ et API**  
- **Repository Git propre**  
- **Roadmap pour les versions futures**  

---

## 7. Conclusion

Ce document fixe un périmètre clair pour QRIS afin d’assurer un développement
progressif, structuré et professionnel.  
Il sert de référence pour mesurer l’avancement du projet, éviter la dispersion
et garantir la cohérence globale du système.

QRIS est conçu pour être un moteur quantitatif complet, tout en restant adapté
à un apprentissage personnel et à la création d’un portfolio de qualité.

