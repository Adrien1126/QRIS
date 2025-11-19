# QRIS — Quantitative Risk & Pricing Engine

QRIS est un moteur quantitatif modulaire conçu pour le **pricing d’options**, le **calcul des sensibilités**, et la **gestion du risque**.
Le projet reproduit l’architecture des systèmes utilisés en salles de marché (quant, risk IT, trading) en combinant :

* un moteur haute performance en **C++**
* une couche de recherche et de prototypage en **Python**
* une API moderne et industrialisable en **.NET**
* une interface utilisateur simple via **Excel**
* une gestion rigoureuse des données de marché via **SQL**
* une documentation complète et professionnelle

QRIS a un double objectif :
**apprentissage avancé** en finance quantitative & **portfolio technique** démontrant des compétences full-stack quant.

---

# 🔍 Vision d’Ensemble

QRIS est conçu comme une plateforme modulaire permettant :

* l’implémentation de modèles de pricing (Black-Scholes, Heston, Monte Carlo…)
* la structuration d’un moteur quant robuste et extensible
* la séparation stricte entre instruments, modèles, pricers et market data
* l’exposition du moteur via une API professionnelle
* la consommation depuis Excel ou une webapp
* l’expérimentation, la recherche et la calibration via Python

Il s’agit d’un **laboratoire personnel**, mais construit selon les standards des institutions financières.

---

# 🏛️ Architecture Globale

Le système est organisé en couches indépendantes :

```
src/core (C++)     → Moteur quantitatif haute performance
src/python         → Recherche, prototypage, validation
src/api (.NET)     → API REST, orchestration, services
src/excel          → Add-in Excel (Excel-DNA)
src/sql            → Market data & schéma SQL
docs/              → Documentation complète
```

Chaque couche peut évoluer indépendamment.

---

# 🧩 Structure du Repository

```
QRIS/
│
├── src/
│   ├── core/                     # MOTEUR QUANTITATIF C++
│   │   ├── instruments/          # Définition des produits (ex: EuropeanOption)
│   │   ├── models/               # Modèles (Black-Scholes, Heston…)
│   │   ├── pricing/              # Méthodes de pricing
│   │   │   ├── black_scholes/    # Module BS (closed-form, Greeks, IV)
│   │   │   ├── heston/           # Placeholder extensible
│   │   │   └── monte_carlo/      # Placeholder extensible
│   │   ├── utils/                # Math utils, normal law, RNG, etc.
│   │   └── tests/                # Tests unitaires C++
│   │
│   ├── api/                      # API .NET
│   ├── excel/                    # Add-in Excel (Excel-DNA)
│   ├── python/                   # Notebooks et recherche
│   └── sql/                      # Market data – schéma + seeds
│
├── docs/                         # Documentation complète
│   ├── overview/
│   ├── core/
│   │   ├── instruments.md
│   │   ├── models.md
│   │   ├── pricing.md
│   │   └── black_scholes/
│   │       ├── design.md
│   │       └── theory.md
│   ├── api/
│   ├── excel/
│   ├── data/
│   ├── roadmap/
│   └── learning/
│
├── samples/                      # Exemples (notebooks, Excel, API)
└── README.md
```

Cette structure reflète **une architecture professionnelle** et prépare l’extension du moteur.

---

# 📦 Modules et Interactions

## 🔹 C++ Core — Moteur Quantitatif

Le cœur du projet, inspiré des moteurs quant internes aux banques :

* **Instruments** : définition abstraite des produits (ex : EuropeanOption)
* **Models** : modèles mathématiques (Black-Scholes, Heston…)
* **Pricing** : méthodes de pricing (closed-form, MC, PDE…)
* **Utils** : outils mathématiques (CDF normale, intégration, RNG…)
* **Tests** : couverture unitaire

Dépendances internes :

```
Instruments → Models → Pricing → Utils
```

Aucun module supérieur ne dépend d’un module inférieur.

---

# 🎯 Focus Actuel : Module **Black-Scholes**

Le premier module complet du moteur.

### Inclus :

* Pricing closed-form call/put
* Calcul de Delta & Gamma
* Solveur de volatilité implicite (IV)
* Séparation stricte :

  * EuropeanOption
  * BlackScholesModel
  * BlackScholesPricer
  * Greeks
  * BlackScholesIVSolver
  * NormalDistribution / MathUtils

### Exemples de diagramme interne :

```
[ EuropeanOption ]
        ↓
[ BlackScholesPricer ]
        ↓
[ BlackScholesModel ] → d1, d2 → NormalDistribution
        ↓
Price + Greeks
        ↑
[ IV Solver ]
```

Ce module sert de **template architectural** pour les modèles futurs (Heston, SABR, MC).

---

# 🧭 Roadmap Technique (par couches)

## 🔹 C++ Core

* [ ] Black-Scholes closed-form
* [ ] Greeks (Delta, Gamma, Vega, Theta)
* [ ] IV solver robuste
* [ ] Monte Carlo framework
* [ ] Modèle Heston
* [ ] Surfaces de volatilité

## 🔹 API .NET

* [ ] Endpoint `/pricing/bs/european`
* [ ] Validation inputs → models Core
* [ ] Bridge C++ (P/Invoke)
* [ ] MarketDataService
* [ ] Documentation Swagger

## 🔹 Excel Add-in

* [ ] Fonction `QRIS.BS(...)`
* [ ] Wrapping API REST
* [ ] Formatage résultat

## 🔹 Python (Recherche)

* [ ] Notebooks de validation BS
* [ ] Comparaison C++ vs Python (sanity checks)
* [ ] Module de calibration (future)

## 🔹 SQL (Market Data)

* [ ] Tables spot / taux / vol
* [ ] Seeds basiques
* [ ] Intégration API

---

# 🎒 Getting Started

Cloner le projet :

```bash
git clone https://github.com/<username>/QRIS.git
cd QRIS
```

Chaque module possède sa documentation associée dans `docs/`.

---

# 📘 Documentation

La documentation complète se trouve dans :

```
docs/
  overview/         → vision globale
  core/             → architecture du moteur C++
  api/              → API .NET
  excel/            → add-in Excel
  data/             → market data
  roadmap/          → versions futures
  learning/         → journal d’apprentissage
```

---

# 👤 Auteur

Développé par **Adrien GLUCKMAN**
Master en Finance Quantitative
Recherche un poste en :

* finance quantitative
* risk management
* quant IT / pricing engine

📩 [adriengluckman892@hotmail.fr](mailto:adriengluckman892@hotmail.fr)
🔗 linkedin.com/in/adrien-gluckman

---

# 📄 Licence

La licence sera définie ultérieurement.

