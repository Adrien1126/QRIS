# QRIS — Quantitative Risk & Pricing Engine

QRIS est un moteur quantitatif modulaire conçu pour le pricing d’options,
le calcul de risque et l’analyse financière.  
Le projet a pour objectif de reproduire l’architecture d’un système utilisé
dans les salles de marché (quant, risk IT, trading) en combinant :

- un cœur haute performance en **C++**
- une couche de recherche et de prototypage en **Python**
- une API moderne en **.NET**
- une intégration utilisateur via **Excel**
- une gestion des données via **SQL**

L’objectif principal est éducatif et professionnel : développer des compétences
solides en finance quantitative, architecture logicielle et ingénierie de
systèmes, tout en construisant un portfolio crédible.

---

## Overview

QRIS (Quantitative Risk & Pricing Engine) est une plateforme destinée à :

- implémenter des modèles de pricing (Black-Scholes, Heston, Monte Carlo…)
- structurer un moteur quant robuste et extensible
- comprendre la séparation entre instruments, modèles, pricers et data
- exposer des services de pricing via une API professionnelle
- offrir un accès utilisateur simple via Excel et une application web
- approfondir la documentation et la théorie mathématique associée

Le projet est pensé comme un **lab d’apprentissage personnel**, mais structuré
suivant les standards des institutions financières.

---

## Architecture du Projet

QRIS repose sur une architecture en couches :

```
C++ Core       → Pricing haute performance
Python         → Recherche, prototypage, ML
.NET API       → Distribution, orchestration, services
Excel Add-in   → Interface utilisateur pour traders/analystes
SQL            → Market Data, stockage et persistance
Documentation  → Architecture + théorie + organisation projet
```

Chaque module est indépendant, modulaire et testable.

---

## 🧩 Fonctionnalités

### Fonctionnalités actuelles
- Structure complète du projet
- Documentation architecture
- Design du module Black-Scholes (en cours)

### Fonctionnalités prévues (Roadmap)
- Pricing Black-Scholes complet (C++)
- Calcul des Greeks
- Inversion de volatilité implicite
- API .NET : endpoint `/pricing/bs`
- Add-in Excel : `QRIS.BS()`
- Module Monte Carlo
- Modèle Heston
- Courbes et surfaces de volatilité
- Market Data provider
- Web dashboard (optionnel)
- Modèles ML (optionnel)

Le projet est volontairement **itératif** : chaque module est construit pour
être étendu sans casser la structure.

---

## Structure du Repository

```
QRIS/
│
├── docs/               # Documentation technique, architecture, théorie
│   ├── architecture/   # Design des modules et flux
│   ├── research/       # Notes mathématiques et financières
│   └── project/        # Organisation, scope, milestones
│
├── cpp/                # Moteur quant haute performance
├── api/                # API .NET pour exposer les services
├── excel_addin/        # Intégration Excel via Excel-DNA
├── sql/                # Schémas et seeds Market Data
└── README.md
```

La documentation est un élément central du projet.

---

## 🚀 Getting Started

Pour cloner le projet :

```bash
git clone https://github.com/<username>/QRIS.git
cd QRIS
```

Le code est organisé de façon à permettre un développement incrémental.
Les instructions d’installation et d’exécution seront ajoutées au fur et à mesure
de l'avancement du projet.

---

## Documentation

La documentation complète est disponible dans le dossier `docs/` :

- `docs/architecture/` — design technique complet
- `docs/project/` — description du périmètre et organisation
- `docs/research/` — théorie quantitative et notes personnelles

Chaque nouvelle fonctionnalité est accompagnée d’une documentation dédiée.

---

## Objectif pédagogique & professionnel

QRIS sert à démontrer des compétences en :

- finance quantitative
- programmation C++ orientée performance
- architecture logicielle modulaire
- API modernes (C# / .NET)
- manipulation de données de marché
- documentation technique
- rigueur, organisation, autonomie

C’est également un support d’apprentissage et d’exploration
mathématique continue.

---

## Auteur

Développé par : **Adrien GLUCKMAN**  
Diplômé d’un Master en Finance Quantitative  
À la recherche d’un poste en finance quantitative / risk / quant IT  
Contact : adriengluckman892@hotmail.fr | linkedin.com/in/adrien-gluckman

---

## Licence

Le projet est en cours de développement.  
La licence sera définie ultérieurement.
