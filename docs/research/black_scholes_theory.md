# Black-Scholes Theory

## 1. Introduction

Le modèle de Black-Scholes-Merton constitue l’un des fondements de la finance quantitative moderne. Introduit dans les années 1970, il fournit la première formule en temps continu permettant d’évaluer le prix d’une option européenne en supposant un marché parfait et une dynamique lognormale du sous-jacent.
Il repose sur deux idées majeures :
la possibilité de construire un portefeuille auto-finançant et delta-neutre,
et le principe de non-arbitrage, qui impose que le rendement de ce portefeuille doit être égal au taux sans risque.
Ces résultats conduisent à une équation aux dérivées partielles (PDE) dont la solution est la célèbre formule fermée Black-Scholes.
Le modèle reste aujourd’hui une référence centrale pour :
la compréhension des dynamiques de marché,
la structuration des modèles plus avancés (Heston, SABR, local vol),
la calibration des surfaces de volatilité,
et la gestion des sensibilités (les “Greeks”).
Bien que ses hypothèses soient souvent trop restrictives pour les marchés réels, il constitue le point d’entrée obligatoire pour tout moteur de pricing.

## 2. Hypothèses du Modèle

Le modèle de Black-Scholes repose sur un ensemble d’hypothèses structurelles concernant le marché, la dynamique du sous-jacent et les conditions économiques. Ces hypothèses sont essentielles car elles conditionnent la validité du raisonnement mathématique qui mène à l’équation de Black-Scholes et à la formule fermée.

Elles peuvent être regroupées en trois catégories :
(1) hypothèses sur le marché,
(2) hypothèses sur les actifs,
(3) hypothèses économiques.

---

## **2.1 Hypothèses de Marché**

### **1. Marché frictionless (sans frictions)**

* Aucun coût de transaction.
* Aucun bid-ask spread.
* Fractionnement infini des positions (trading continu).
* Possibilité de vendre à découvert sans contraintes.

➡️ *Justification :* permet la réplication exacte par une stratégie delta-hedging.

👉 **Source :**
Black & Scholes (1973), Section II ; Merton (1973).

---

### **2. Trading continu et liquidité parfaite**

* Le portefeuille est rééquilibré en continu.
* Les actifs peuvent être achetés ou vendus instantanément dans n’importe quelle quantité.

➡️ *Justification :* nécessaire pour neutraliser totalement le risque via la couverture.

👉 **Source :**
Shreve (2004), Chapitre 7 ; Björk (2009), Chapitre 9.

---

## **2.2 Hypothèses sur le Sous-jacent**

### **3. Dynamique géométrique brownienne (GBM)**

Le prix du sous-jacent suit une SDE (sous la mesure réelle) :

$$
dS_t = \mu S_t , dt + \sigma S_t , dW_t
$$

où :

* ( $\mu$ ) est le drift réel,
* ( $\sigma$) est la volatilité, constante,
* ( $W_t$ ) est un mouvement brownien standard.

➡️ *Justification :* conduit à un processus lognormal simple à manipuler.

👉 **Source :**
Shreve (2004), Chapitre 5 ; Hull, Chapitre 13.

---

### **4. Absence de sauts dans les prix**

Le prix évolue de manière continue.

➡️ *Conséquence :* limite importante du modèle (réaliste uniquement pour actifs très liquides hors annonces).
➡️ *Prépare les extensions* : Merton Jump-Diffusion, Bates, Variance Gamma.

👉 **Source :**
Bjork (2009), Section 10.2.

---

### **5. Volatilité constante**

La volatilité instantanée ( $\sigma$ ) est supposée **constante dans le temps et indépendante du prix**.

➡️ *Conséquence :* smile/skew ignoré → limite majeure en pratique.
➡️ *Justification structurelle :* permet une solution fermée.

👉 **Source :**
Hull, Sect. 15 ; Lamberton & Lapeyre.

---

## **2.3 Hypothèses Économiques**

### **6. Taux d’intérêt constant et connu**

Le taux sans risque ( $r$ ) est constant dans le temps.

➡️ *Justification :* nécessaire pour construire la mesure risque-neutre.
➡️ *Conséquence :* ignore la courbe de taux (interpolation nécessaire en pratique).

👉 **Source :**
Merton (1973), Section “Riskless Asset”.

---

### **7. Absence d’opportunité d’arbitrage**

Il n’existe pas de stratégie auto-finançante offrant un profit sans risque.

➡️ *Axiome fondamental* : utilisé pour dériver la PDE.
➡️ *Conséquence :* prix unique de l’option.

👉 **Source :**
Fundamental Theorem of Asset Pricing — Shreve (2004), Chapitre 6.

---

### **8. Marché complet**

Tout payoff contingent peut être répliqué de manière unique via une stratégie de trading.

➡️ *Justification :* le modèle GBM est un marché complet.
➡️ *Conséquence :* existence et unicité du prix.

👉 **Source :**
Bjork (2009), Chapitre 6 ; Duffie (2001).

---

### **9. Dividendes inexistants ou taux de dividende constant**

Version de base : pas de dividende.
Version pratique : dividende continu ( q ) constant.

➡️ *Prépare l’extension Black-Scholes-Merton*.

👉 **Source :**
Merton (1973) — modèle avec dividende.

---

# 🔍 **Résumé des hypothèses clefs (à mettre éventuellement en tableau plus tard)**

| Catégorie   | Hypothèse                                                          |
| ----------- | ------------------------------------------------------------------ |
| Marché      | Frictionless, trading continu, liquidité parfaite                  |
| Sous-jacent | GBM, volatilité constante, pas de sauts                            |
| Économie    | r constant, pas d’arbitrage, marché complet, dividendes optionnels |

---

## 3. Dynamique du Sous-Jacent

Le modèle de Black-Scholes suppose que le prix du sous-jacent ( S_t ) suit un **mouvement brownien géométrique (GBM)** sous la mesure réelle. Cette hypothèse est centrale : elle définit la dynamique du prix, la distribution en maturité et l’architecture mathématique qui conduit à la formule fermée.

Cette section détaille :

* la justification économique de la dynamique,
* la SDE sous la mesure réelle,
* la solution explicite,
* les propriétés fondamentales,
* l’interprétation des paramètres,
* la transition vers la mesure risque-neutre (préparation de la section 4).

---

## **3.1 Équation différentielle stochastique**

Le sous-jacent suit la SDE suivante :

$$
dS_t = \mu S_t, dt + \sigma S_t, dW_t
$$

avec :

* ( $\mu$ ) : drift réel (taux d’espérance de rendement),
* ( $\sigma > 0$) : volatilité instantanée, constante,
* ( $W_t$ ) : mouvement brownien standard.

Cette équation est du type **diffusion multiplicative**, ce qui implique que les variations relatives ( $dS_t / S_t$ ) sont normales.

---

## **3.2 Justification économique**

Le choix d’une dynamique proportionnelle :

$$
\frac{dS_t}{S_t} = \mu, dt + \sigma, dW_t
$$

est motivé par :

1. **Retour proportionnel au prix**
   Un actif qui vaut 100 et un actif qui vaut 10 ne réagissent pas de la même manière aux mêmes chocs.
   La volatilité relative est plus stable que la volatilité absolue.

2. **Croissance exponentielle attendue**
   Le rendement composé continuellement est plus proche de la réalité des marchés.

3. **Empiriquement :**
   les rendements log sont plus proches de la normalité que les prix eux-mêmes.

---

## **3.3 Solution explicite de la SDE**

La SDE de Black-Scholes est l’une des rares équations stochastiques linéaires à avoir une solution exacte.

En appliquant Itô au log du prix, on obtient la solution :

$$
S_t = S_0 \exp\left( \left(\mu - \frac{1}{2}\sigma^2\right)t + \sigma W_t \right)
$$

Il en découle immédiatement :

$$
\ln S_t \sim \mathcal{N}\left(
\ln S_0 + \left(\mu - \frac{1}{2}\sigma^2\right)t,;
\sigma^2 t
\right)
$$

Cette distribution log-normale est au cœur de la formule de pricing.

---

## **3.4 Propriétés fondamentales du processus**

### **1. Distribution lognormale**

Le prix est toujours strictement positif :
$$
S_t > 0 \quad \text{pour tout } t.
$$

### **2. Variance proportionnelle au temps**

$$
\operatorname{Var}[\ln S_t] = \sigma^2 t.
$$

### **3. Espérance exponentielle**

$$
\mathbb{E}[S_t] = S_0 e^{\mu t}.
$$

### **4. Proportionnalité du risque**

La volatilité absolue dépend du niveau du prix.
C’est psychologiquement et économiquement réaliste.

---

## **3.5 Interprétation des paramètres**

### **Le drift ( $\mu$ )**

* représente la croissance moyenne du prix sous la *mesure réelle*.
* n’intervient **pas** dans la formule de pricing (sous mesure risque-neutre).

### **La volatilité ( $\sigma$ )**

* mesure la dispersion des rendements.
* paramètre central du modèle.
* considéré constant → déviation majeure par rapport aux marchés.

### **Le brownien ( $W_t$ )**

* représente l’incertitude fondamentale.
* processus continu, à accroissements indépendants.

---

## **3.6 Vers la mesure risque-neutre (aperçu)**

La transition vers la mesure risque-neutre (section 4) repose sur :

* la possibilité de se couvrir continuellement,
* l’absence d’arbitrage,
* et le choix du numéraire (actif sans risque).

Sous la mesure risque-neutre :

$$
dS_t = r S_t dt + \sigma S_t dW_t^{\mathbb{Q}}
$$

où :

* le drift devient **le taux sans risque**,
* la distribution reste lognormale,
* le paramètre clé reste ( $\sigma$ ).

## 4. Passage au Numéraire et Mesure Risque-Neutre

Dans un marché frictionless avec un actif sans risque ( $B_t = e^{rt}$ ), tout actif risqué ( $S_t$ ) peut être exprimé dans l’unité de compte définie par ( $B_t$ ).
On définit alors le processus **actualisé** :

$$
\tilde{S}_t = \frac{S_t}{B_t}.
$$

La condition d’absence d’arbitrage (no free lunch with vanishing risk) impose que **tout prix actualisé doit être une martingale sous une certaine mesure équivalente** à la mesure réelle.

Ce résultat découle du **Premier théorème fondamental de l'absence d'arbitrage**, démontré rigoureusement dans :

* Harrison & Kreps (1979),
* Harrison & Pliska (1981),
* Björk (2009), Chap. 6.

Sous cette mesure équivalente ( $\mathbb{Q}$ ), dite **mesure neutre au risque associée au numéraire ( $B_t$ )**, le processus ( $\tilde{S}_t$ ) doit satisfaire :

$$
\tilde{S}_t = \mathbb{E}^{\mathbb{Q}} \left[ \tilde{S}_T \mid \mathcal{F}_t \right].
$$

Cette condition caractérise la mesure risque-neutre de manière **structurelle**, sans faire référence à un argument économique.

---

# **4.1 Dynamique sous la mesure réelle**

Le prix du sous-jacent suit sous ( $\mathbb{P}$ ) :

$$
dS_t = \mu S_t , dt + \sigma S_t , dW_t^{\mathbb{P}}.
$$

On en déduit la dynamique du prix actualisé :

$$
d\tilde{S}_t = (\mu - r)\tilde{S}_t, dt + \sigma \tilde{S}_t, dW_t^{\mathbb{P}}.
$$

---

# **4.2 Construction formelle de la mesure ( $\mathbb{Q}$ )**

L'objectif est de définir une mesure ( $\mathbb{Q} \sim \mathbb{P}$ ) telle que ( $\tilde{S}_t$ ) soit une martingale.

On cherche un processus brownien ( $W_t^{\mathbb{Q}}$ ) tel que :

$$
d\tilde{S}_t = \sigma \tilde{S}_t , dW_t^{\mathbb{Q}}.
$$

On compare avec la dynamique sous ( $\mathbb{P}$ ) :

$$
d\tilde{S}_t = (\mu - r)\tilde{S}_t dt + \sigma \tilde{S}_t dW_t^{\mathbb{P}}.
$$

On constate que la partie dérive doit disparaître.
Le théorème de **Girsanov** fournit exactement le changement de mesure adapté.

Définissons le processus :

$$
\theta = \frac{\mu - r}{\sigma}.
$$

Le changement de mesure de Radon–Nikodym :

$$
\frac{d\mathbb{Q}}{d\mathbb{P}} = \exp\left(
-\theta W_T^{\mathbb{P}} - \frac{1}{2}\theta^2 T
\right)
$$

garantit que :

$$
W_t^{\mathbb{Q}} = W_t^{\mathbb{P}} + \theta t
$$

est un Brownien sous ( $\mathbb{Q}$ ).

Cette construction est standard, cf. :

* **Karatzas & Shreve**, *Brownian Motion and Stochastic Calculus* (1991)
* **Shreve**, *Stochastic Calculus for Finance II*, Chap. 6
* **Björk**, Chap. 10.

En substituant :

$$
dW_t^{\mathbb{P}} = dW_t^{\mathbb{Q}} - \theta dt,
$$

la SDE actualisée devient :

$$
d\tilde{S}_t = \sigma \tilde{S}_t , dW_t^{\mathbb{Q}}.
$$

La dérive a disparu → martingale.

---

# **4.3 Dynamique sous la mesure risque-neutre**

Reconstruisons la dynamique de ( $S_t$ ) :

$$
S_t = B_t \tilde{S}_t.
$$

Comme ( $dB_t = rB_t dt$ ), on obtient :

$$
dS_t = r S_t dt + \sigma S_t dW_t^{\mathbb{Q}}.
$$

C’est la dynamique *canonique* sous la mesure risque-neutre :

$$
\boxed{
dS_t = r S_t dt + \sigma S_t dW_t^{\mathbb{Q}}
}
$$

Elle remplace ( $\mu$ ) par ( $r$ ), alors que :

* la volatilité reste inchangée
* la structure stochastique est conservée
* seules les probabilités changent

---

# **4.4 Conséquence : évaluation neutre au risque**

Pour tout payoff mesurable ( $H(S_T)$ ), le prix sans arbitrage est :

$$
V_t = B_t \mathbb{E}^{\mathbb{Q}} \left[ \frac{H(S_T)}{B_T} \mid \mathcal{F}_t \right]
=e^{-r(T-t)} \mathbb{E}^{\mathbb{Q}}[H(S_T) \mid \mathcal{F}_t].
$$

Il s’agit du **formalisme général de pricing sous la mesure risque-neutre**.

Ce résultat constitue la base :

* de la formule de Black-Scholes,
* de la méthode Monte Carlo,
* des modèles stochastiques (Heston),
* du pricing des taux (choix du numéraire),
* du pricing en forward measure (numéraire ( B(t,T) )),
* des techniques de change de mesure (Girsanov, Margrabe, etc.).

---

# **4.5 Généralisation (numéraires alternatifs)**

Black–Scholes est un cas particulier :
numéraire = actif sans risque.

Mais on peut choisir d’autres numéraires :

### • Numéraire forward : ( $P(t,T)$ )

→ évaluation en *T-forward measure*, utilisée en fixed income.

### • Numéraire sous-jacent : ( $S_t$ )

→ *equity forward measure*, utile pour les change-of-numéraire tricks.

### • Numéraire FX : compte domestique / compte étranger

→ pricing des dérivés de change.

# 📚 **Sources vérifiables**

### Ouvrages fondamentaux

* Karatzas & Shreve — *Brownian Motion and Stochastic Calculus*, Chap. 5
* Shreve — *Stochastic Calculus for Finance II*, Chap. 6–7
* Björk — *Arbitrage Theory in Continuous Time*, Chap. 10

### Articles structurants

* Harrison & Pliska (1981) — *Martingales and Arbitrage in Multiperiod Securities Markets*
* Geman, El Karoui & Rochet (1995) — *Changes of Numéraire, Changes of Probability Measure and Option Pricing*

## **5. Processus Log-Normal**

Dans le cadre du modèle de Black–Scholes, la dynamique du sous-jacent sous la mesure risque-neutre ( $\mathbb{Q}$ ) est :

$$
dS_t = r S_t, dt + \sigma S_t, dW_t^{\mathbb{Q}},
\qquad S_0 > 0.
\tag{5.1}
$$

C’est une équation différentielle stochastique linéaire multiplicative.
Cette section établit :

1. sa **solution explicite** via la formule d’Itô,
2. la **distribution exacte** du processus,
3. les **moments**, **cumulants**, **densité**,
4. les propriétés de **martingale**,
5. la justification mathématique permettant le pricing.

---

# **5.1 Linéarisation de la SDE par transformation logarithmique**

Considérons la fonction ( $f(x) = \ln x$ ) (bien définie car ( $S_t > 0$ )).
Appliquons la formule d’Itô à ( $X_t = f(S_t) = \ln S_t$ ).

On obtient :

$$
dX_t = \left( r - \frac{1}{2}\sigma^2 \right) dt + \sigma, dW_t^{\mathbb{Q}}.
\tag{5.2}
$$

En effet, pour ( $f(x)=\ln x$ ) :

* ( $f'(x) = \frac{1}{x}$ ),
* ( $f''(x) = -\frac{1}{x^2}$ ).

En appliquant Itô :

$$
d(\ln S_t)
= \frac{1}{S_t} dS_t

* \frac{1}{2}\frac{1}{S_t^2} ( \sigma^2 S_t^2 ) dt
  = \left( r - \frac{1}{2}\sigma^2 \right) dt

- \sigma dW_t^{\mathbb{Q}}.
$$

Cela montre que **le logarithme du prix suit un mouvement brownien avec drift constant**.

---

# **5.2 Solution explicite**

L’équation (5.2) s’intègre directement :

$$
\ln S_t
=
\ln S_0 + \left( r - \frac{1}{2}\sigma^2 \right)t + \sigma W_t^{\mathbb{Q}}.
\tag{5.3}
$$

En exponentiant :

$$
\boxed{
S_t
=
S_0
\exp\left(
\left( r - \frac{1}{2}\sigma^2 \right)t +
\sigma W_t^{\mathbb{Q}}
\right).
}
\tag{5.4}
$$

C’est la **solution exacte** du modèle Black–Scholes.
Elle ne dépend ni d’approximations ni de discrétisation.

---

# **5.3 Distribution exacte**

Comme ( $W_t^{\mathbb{Q}} \sim \mathcal{N}(0, t)$ ), il en résulte que :

$$
\ln S_t \sim \mathcal{N}
\left(
\ln S_0 + \left( r - \frac{1}{2}\sigma^2 \right)t, \quad \sigma^2 t
\right).
\tag{5.5}
$$

Donc :

$$
S_t \text{ suit une distribution log-normale.}
$$

Sa densité transitionnelle est :

$$
p(t, S_0, s)
=
\frac{1}{ \sigma\sqrt{2\pi t}}
\exp\left(
-\frac{\left[
\ln(s/S_0) - (r - \frac{1}{2}\sigma^2)t
\right]^2}{2\sigma^2 t}
\right).
\tag{5.6}
$$

Cette densité intervient dans l’intégrale fermée de Black–Scholes.

---

# **5.4 Moments, espérance, variance**

### **Espérance sous la mesure risque-neutre**

À partir de la solution explicite :

$$
\mathbb{E}^{\mathbb{Q}}[S_t]
=
S_0 e^{rt}.
\tag{5.7}
$$

Cela reflète que le drift sous ( $\mathbb{Q}$ ) est bien ( $r$ ).

---

### **Variance**

$$
\operatorname{Var}^{\mathbb{Q}}[S_t]
=
S_0^2 e^{2rt}
\left( e^{\sigma^2 t} - 1 \right).
\tag{5.8}
$$

---

### **Moments log-normaux**

Pour tout ( $p \in \mathbb{R}$ ) :

$$
\mathbb{E}[S_t^p]
=
S_0^p \exp\left(
p\left(r - \frac{1}{2}\sigma^2\right)t

* \frac{1}{2} p^2 \sigma^2 t
  \right).
  \tag{5.9}
$$

Cette formule est utile pour :

* pricing d’options de puissance,
* méthodes semi-analytiques,
* calcul d’entropie.

---

# **5.5 Martingale sous ( $\mathbb{Q}$ )**

On définit :

$$
\tilde{S}_t = e^{-rt} S_t.
\tag{5.10}
$$

À partir de (5.4), on obtient :

$$
\tilde{S}_t
=

S_0
\exp\left(
-\frac{1}{2}\sigma^2 t + \sigma W_t^{\mathbb{Q}}
\right).
$$

On vérifie :

$$
\mathbb{E}^{\mathbb{Q}}[\tilde{S}_t]
=
S_0.
\tag{5.11}
$$

Cette propriété est **nécessaire et suffisante** pour l’absence d’arbitrage lorsque le numéraire est ( $B_t$ ).
Elle est au cœur du théorème fondamental de l'évaluation neutre au risque.

---

# **5.6 Implications pour la formule de Black–Scholes**

Grâce à la distribution log-normale :

$$
S_T
=
S_0 \exp\left(
\left(r - \frac{1}{2}\sigma^2\right)T + \sigma \sqrt{T} Z
\right),
\qquad Z \sim \mathcal{N}(0,1),
$$

il devient possible d’évaluer **exactement** :

$$
\mathbb{E}^{\mathbb{Q}}
\left[
(S_T - K)^+
\right]
$$

sans recourir à une simulation.

Cette intégrale conduit à :

* la formule fermée de Black–Scholes pour un call,
* la formule put via parité,
* les expressions de ( $d_1$ ), ( $d_2$ ),
* les greeks fermés.

C’est la **raison fondamentale** pour laquelle
le modèle Black–Scholes est analytiquement soluble.

---

## 6. Construction de la Stratégie de Couverture

La stratégie de couverture est au cœur du modèle Black–Scholes.
Elle permet de dériver la PDE de pricing en montrant :

1. qu’un instrument dérivé est réplicable,
2. que la réplication élimine le terme stochastique,
3. que le rendement du portefeuille couvert doit être égal au taux sans risque,
4. ce qui impose une équation aux dérivées partielles unique.

Nous formalisons cette construction étape par étape.

---

# **6.1 Notations et cadre**

Considérons :

* un actif risqué ( $S_t$ ) suivant sous ( $\mathbb{Q}$ ) :

$$
dS_t = r S_t dt + \sigma S_t dW_t^{\mathbb{Q}},
\tag{6.1}
$$

* un actif dérivé de valeur ( $V(S_t,t)$ ), supposée ( $C^{2,1}$ ) au sens classique (dérivable deux fois en ( $S$ ), une fois en ( $t$ )).

Le but est de construire un portefeuille **auto-finançant** ayant le même payoff que l’option.

---

# **6.2 Application d’Itô à la valeur du dérivé**

En appliquant la formule d’Itô à ( $V(S_t,t)$ ), on obtient :

$$
dV =
\left(
\frac{\partial V}{\partial t}
+
\frac{1}{2}\sigma^2 S_t^2 \frac{\partial^2 V}{\partial S^2}
+
r S_t \frac{\partial V}{\partial S}
  \right) dt
+
\sigma S_t \frac{\partial V}{\partial S}
, dW_t^{\mathbb{Q}}.
\tag{6.2}
$$

Cette décomposition sépare la contribution :

* **déterministe** (drift),
* **stochastique** (terme en ( $dW_t^{\mathbb{Q}}$ )).

L’objectif de la couverture est précisément **d’éliminer ce terme stochastique**.

---

# **6.3 Portefeuille auto-finançant**

Construisons un portefeuille ( $\Pi_t$ ) contenant :

* ( $\Delta_t$ ) unités du sous-jacent ( $S_t$ ),
* et ( $\beta_t$ ) unités de l’actif sans risque.

Valeur du portefeuille :

$$
\Pi_t = \Delta_t S_t + \beta_t B_t.
$$

Sa variation (auto-finançante) est :

$$
d\Pi_t = \Delta_t dS_t + \beta_t dB_t.
\tag{6.3}
$$

Avec ( $dB_t = r B_t dt$ ).

---

# **6.4 Choix de la stratégie de couverture**

On choisit :

$$
\Delta_t = \frac{\partial V}{\partial S}(S_t,t).
$$

Ce choix annule le terme en ( $dW_t^{\mathbb{Q}}$ ) dans la différence entre ( $dV$ ) et ( $d\Pi_t$ ).

En effet, en substituant (6.1) dans (6.3) :

$$
d\Pi_t
=
\Delta_t (r S_t dt + \sigma S_t dW_t^{\mathbb{Q}} )
+ \beta_t r B_t dt.
$$

En choisissant :

$$
\Delta_t = \frac{\partial V}{\partial S},
$$

le terme stochastique dans ( $dV - d\Pi_t$ ) devient :

$$
\sigma S_t \frac{\partial V}{\partial S} dW_t^{\mathbb{Q}}
-
\Delta_t \sigma S_t dW_t^{\mathbb{Q}}
= 0.
$$

On a donc construit **un portefeuille parfaitement couvert**, sans risque infinitésimal.

---

# **6.5 Argument de non-arbitrage**

Comme ( $dV - d\Pi_t$ ) n’a plus de composante stochastique, il doit évoluer au **taux sans risque**, faute de quoi une stratégie d’arbitrage serait possible.

On impose donc :

$$
dV = r V dt.
\tag{6.4}
$$

En remplaçant ( $dV$ ) par son expression issue d’Itô (6.2), on obtient :

$$
\frac{\partial V}{\partial t}
+
\frac{1}{2}\sigma^2 S_t^2 \frac{\partial^2 V}{\partial S^2}
+ r S_t \frac{\partial V}{\partial S}
=
r V.
\tag{6.5}
$$

---

# **6.6 PDE de Black–Scholes**

On réarrange (6.5) :

$$
\boxed{
\frac{\partial V}{\partial t}
+
\frac{1}{2}\sigma^2 S^2 \frac{\partial^2 V}{\partial S^2}
+
r S \frac{\partial V}{\partial S}
- r V = 0.
}
\tag{6.6}
$$

C’est **l’équation de Black–Scholes**, base de la solution fermée.

---

# **6.7 Condition terminale**

Pour une option européenne de payoff ( $H(S_T)$ ), la condition terminale est :

$$
V(S,T) = H(S).
\tag{6.7}
$$

Le couple :

* PDE (6.6),
* condition terminale (6.7),

forme un problème de Cauchy bien posé, admettant une **solution analytique unique**.

---

# **6.8 Interprétation : existence et unicité du prix**

La stratégie précédente montre :

* la **réplicabilité** du payoff (marché complet),
* l’**unicité** du prix sans arbitrage,
* que ce prix doit suivre la solution de la PDE (théorème de Feynman–Kac à vérifier dans Section 17).

L’équation de Black–Scholes est donc **une conséquence directe** :

* de la dynamique stochastique du sous-jacent,
* de l’absence d’arbitrage,
* de la possibilité de trader continûment,
* et de la différentiabilité du prix.

---

## 7. Résolution de l’Équation de Black-Scholes

L’objectif est de résoudre le problème de Cauchy suivant :

$$
\begin{cases}
V_t + \frac{1}{2}\sigma^2 S^2 V_{SS} + r S V_S - r V = 0, \
V(S,T) = H(S).
\end{cases}
\tag{9.1}
$$

On montre que cette PDE peut être transformée en l’**équation de la chaleur** classique :

$$
u_\tau = u_{xx}.
\tag{9.2}
$$

Nous suivons rigoureusement la méthode standard :

* (i) *reparamétrisation en temps inversé*,
* (ii) *logarithmisation du sous-jacent*,
* (iii) *normalisation de la PDE*,
* (iv) *résolution par convolution gaussienne*.

---

# **7.1 Étape 1 : Transformation du temps**

On introduit un temps inversé :

$$
\tau = T - t,
$$

de sorte que la condition terminale devienne **une condition initiale** :

$$
V(S,T) = H(S)
\quad \Longleftrightarrow \quad
U(S,0) = H(S),
$$

où l’on définit :

$$
U(S,\tau) = V(S, T - \tau).
$$

Alors :

$$
U_\tau = -V_t.
\tag{9.3}
$$

En substituant dans (9.1) :

$$
-U_\tau
+
\frac{1}{2}\sigma^2 S^2 U_{SS}
+
r S U_S
- 
r U = 0.
\tag{9.4}
$$

---

# **7.2 Étape 2 : Transformation logarithmique du sous-jacent**

Définissons :

$$
x = \ln S.
$$

Alors la fonction transformée est :

$$
u(x,\tau) = U(e^x,\tau).
\tag{9.5}
$$

En appliquant la formule de dérivation par changement de variable :

* ( $U_S = e^{-x} u_x$ ),
* ( $U_{SS} = e^{-2x}(u_{xx} - u_x)$ ).

La PDE (9.4) devient :

$$
-u_\tau

* \frac{1}{2}\sigma^2 (u_{xx} - u_x)
* r (u_x)

- r u = 0.
  \tag{9.6}
  $$

---

# **7.3 Étape 3 : Normalisation de la PDE**

Regroupons les termes en ( $u_x$ ) :

$$
-u_\tau

* \frac{1}{2}\sigma^2 u_{xx}
* \left(r - \frac{1}{2}\sigma^2\right) u_x

- r u = 0.
  \tag{9.7}
  $$

L’objectif est d’éliminer

* (i) le terme en dérivée première,
* (ii) le terme multiplicatif en ( $u$ ),
  pour obtenir une équation de la chaleur.

---

# **7.4 Étape 4 : Transformation de type “affine + pondération exponentielle”**

On introduit la fonction :

$$
u(x,\tau) = e^{\alpha x + \beta \tau} w(x,\tau),
\tag{9.8}
$$

où ( $\alpha, \beta$ ) seront choisis pour simplifier la PDE.

### Dérivées :

$$
u_x = e^{\alpha x + \beta\tau}(\alpha w + w_x)
$$

$$
u_{xx} = e^{\alpha x + \beta\tau}(\alpha^2 w + 2\alpha w_x + w_{xx})
$$

$$
u_\tau = e^{\alpha x + \beta\tau}(\beta w + w_\tau)
$$

Substituons ces expressions dans (9.7) et simplifions.
On obtient une équation de la forme :

$$
w_\tau = A w_{xx} + B w_x + C w.
$$

On cherche à imposer **$B = 0$**
et **$C = 0$**,
de manière à obtenir :

$$
w_\tau = A w_{xx}.
$$

---

# **7.5 Choix des paramètres ($\alpha, \beta$)**

Les conditions imposent :

$$
\alpha = \frac{1}{\sigma^2}\left(r - \frac{1}{2}\sigma^2\right),
\qquad
\beta = \frac{\alpha^2 \sigma^2}{2} + r.
\tag{9.9}
$$

Avec ce choix, la PDE se réduit à :

$$
w_\tau = \frac{1}{2}\sigma^2 w_{xx}.
\tag{9.10}
$$

C’est **l’équation de la chaleur classique**.

---

# **7.6 Résolution via le noyau fondamental**

L’équation de la chaleur :

$$
w_\tau = \kappa w_{xx},
\qquad \kappa = \frac{1}{2}\sigma^2,
\tag{9.11}
$$

avec condition initiale :

$$
w(x,0) = e^{-\alpha x} H(e^x),
\tag{9.12}
$$

a pour solution :

$$
w(x,\tau)
=
\frac{1}{\sqrt{4\pi \kappa \tau}}
\int_{\mathbb{R}}
\exp\left(
-\frac{(x-y)^2}{4\kappa \tau}
\right)
e^{-\alpha y} H(e^y) ; dy.
\tag{9.13}
$$

C’est la solution fondamentale de l’équation de la chaleur, via convolution gaussienne.

---

# **7.7 Expression pour ( $V(S,t)$ )**

En réappliquant successivement :

* la transformation ( $u \mapsto U$ ),
* ( $x = \ln S$ ),
* la pondération exponentielle,
* la transformation de temps ( $\tau = T - t$ ),

on obtient :

$$
V(S,t)
=
e^{-(r-\frac{1}{2}\sigma^2)(T-t)} S^\alpha
\frac{1}{\sqrt{2\pi\sigma^2 (T-t)}}
\int_{\mathbb{R}}
\exp\left(
-\frac{(\ln S - y)^2}{2\sigma^2 (T-t)}
\right)
H(e^y) e^{-\alpha y} ; dy.
\tag{9.14}
$$

Cette formule générale est applicable à tout payoff européen régulier.
En particulier :

* pour ( $H(s) = (s - K)^+$ ),
* l’intégrale se ramène à deux intégrales sur une gaussienne,
* ce qui donne la **formule fermée Black–Scholes**.

## 8. Formule Fermée du Prix d’un Call Européen

On considère une option call européenne de maturité (T) et de strike (K), sans dividende, sur un sous-jacent (S) suivant sous la mesure risque-neutre ($\mathbb{Q}$) la dynamique :

$$
dS_t = r S_t dt + \sigma S_t dW_t^{\mathbb{Q}}, \qquad S_0 > 0.
$$

La solution explicite (section 5) est :

$$
S_T = S_0 \exp\left(\left(r - \frac{1}{2}\sigma^2\right)T + \sigma \sqrt{T}, Z\right),
\qquad Z \sim \mathcal{N}(0,1) \text{ sous } \mathbb{Q}.
$$

Le payoff du call est ($H(S_T) = (S_T - K)^+$).
Le prix sans arbitrage à ($t=0$) est donné par l’évaluation neutre au risque :

$$
C_0 = e^{-rT} , \mathbb{E}^{\mathbb{Q}}!\left[(S_T - K)^+\right].
\tag{10.1}
$$

Nous allons dériver explicitement la formule fermée en partant de cette espérance.

---

## 8.1 Écriture intégrale du prix du call

Écrivons le prix comme une intégrale par rapport à la densité de ($S_T$) :

$$
C_0
= e^{-rT} \int_{K}^{\infty} (s - K) f_{S_T}(s) , ds,
\tag{10.2}
$$

où ($f_{S_T}$) est la densité log-normale de ($S_T$).
Une approche plus efficace consiste à travailler directement via la représentation en ($Z$).

On utilise :
$$
S_T = S_0 \exp\left(\left(r - \frac{1}{2}\sigma^2\right)T + \sigma \sqrt{T}, Z\right),\quad Z \sim \mathcal{N}(0,1).
$$

Alors :

$$
C_0
= e^{-rT} , \mathbb{E}^{\mathbb{Q}}!\left[(S_T - K)^+\right]
= e^{-rT} , \mathbb{E}^{\mathbb{Q}}!\left[(S_T - K),\mathbf{1}_{{S_T > K}}\right].
\tag{10.3}
$$

On introduit le seuil en termes de ($Z$).
La condition ($S_T > K$) est équivalente à :

$$
S_0 \exp\left(\left(r - \tfrac{1}{2}\sigma^2\right)T + \sigma\sqrt{T} Z\right) > K
$$

$$
\Longleftrightarrow\quad
Z > z^* := \frac{\ln\left(\frac{K}{S_0}\right) - \left(r - \frac{1}{2}\sigma^2\right)T}{\sigma \sqrt{T}}.
\tag{10.4}
$$

On obtient donc

$$
C_0
= e^{-rT} , \mathbb{E}^{\mathbb{Q}}!\left[(S_T - K),\mathbf{1}_{{Z>z^*}}\right].
\tag{10.5}
$$

En développant :

$$
C_0
= e^{-rT} \left(
\mathbb{E}^{\mathbb{Q}}!\left[S_T \mathbf{1}_{{Z>z^*}}\right]
-
K , \mathbb{E}^{\mathbb{Q}}!\left[\mathbf{1}_{{Z>z^*}}\right]
\right).
\tag{10.6}
$$

Le second terme est immédiat :

$$
\mathbb{E}^{\mathbb{Q}}!\left[\mathbf{1}_{{Z>z^*}}\right]
= \mathbb{P}^{\mathbb{Q}}(Z>z^*)
= N(-z^*),
\tag{10.7}
$$

où ($N$) est la fonction de répartition de la loi normale standard.

On définira plus loin ($d_2 = -z^*$).

Le cœur de la dérivation réside dans le calcul du premier terme
($\mathbb{E}^{\mathbb{Q}}[S_T \mathbf{1}_{{Z>z^*}}]$).

---

## 8.2 Calcul de ($\mathbb{E}^{\mathbb{Q}}[S_T \mathbf{1}_{{Z>z^*}}]$)

En utilisant la représentation de ($S_T$) :

$$
\mathbb{E}^{\mathbb{Q}}[S_T \mathbf{1}_{{Z>z^*}}]
=
\mathbb{E}^{\mathbb{Q}}\left[
S_0 \exp\left(\left(r - \frac{1}{2}\sigma^2\right)T + \sigma \sqrt{T}, Z\right)
\mathbf{1}_{{Z>z^*}}
\right].
$$

On factorise les termes constants :

$$
\mathbb{E}^{\mathbb{Q}}[S_T \mathbf{1}_{{Z>z^*}}]
=
S_0 e^{\left(r - \frac{1}{2}\sigma^2\right)T}
\mathbb{E}^{\mathbb{Q}}\left[
e^{\sigma\sqrt{T} Z} \mathbf{1}_{{Z>z^*}}
\right].
\tag{10.8}
$$

La loi de ($Z$) est standard normale, de densité ($\varphi(z) = \frac{1}{\sqrt{2\pi}} e^{-z^2/2}$).
On écrit donc :

$$
\mathbb{E}^{\mathbb{Q}}\left[
e^{\sigma\sqrt{T} Z} \mathbf{1}_{{Z>z^*}}
\right]
=
\int_{z^*}^{\infty} e^{\sigma\sqrt{T} z} \varphi(z), dz.
\tag{10.9}
$$

On utilise le classique “completion of the square” :

$$
e^{\sigma\sqrt{T} z} \varphi(z)
=
# \frac{1}{\sqrt{2\pi}} \exp\left(\sigma\sqrt{T} z - \frac{z^2}{2}\right)

\frac{1}{\sqrt{2\pi}}
\exp\left(
-\frac{1}{2}\bigl[z^2 - 2\sigma\sqrt{T} z\bigr]
\right).
$$

On complète le carré :

$$
z^2 - 2\sigma\sqrt{T} z
=
(z - \sigma\sqrt{T})^2 - \sigma^2 T.
$$

Donc :

$$
e^{\sigma\sqrt{T} z} \varphi(z)
=
\frac{1}{\sqrt{2\pi}}
\exp\left(
-\frac{1}{2}(z - \sigma\sqrt{T})^2 + \frac{1}{2}\sigma^2 T
\right)
=
e^{\frac{1}{2}\sigma^2 T}
\frac{1}{\sqrt{2\pi}}
\exp\left(
-\frac{1}{2}(z - \sigma\sqrt{T})^2
\right).
$$

Autrement dit :

$$
e^{\sigma\sqrt{T} z} \varphi(z)
=
e^{\frac{1}{2}\sigma^2 T} , \varphi(z - \sigma\sqrt{T}).
\tag{10.10}
$$

En réinjectant dans (10.9) :

$$
\mathbb{E}^{\mathbb{Q}}\left[
e^{\sigma\sqrt{T} Z} \mathbf{1}_{{Z>z^*}}
\right]
=
e^{\frac{1}{2}\sigma^2 T}
\int_{z^*}^{\infty} \varphi(z - \sigma\sqrt{T}), dz.
$$

On effectue le changement de variable ($u = z - \sigma\sqrt{T}$) :

* lorsque ($z = z^*$), ($u = z^* - \sigma\sqrt{T}$),
* lorsque ($z \to \infty$), ($u \to \infty$),
* ($dz = du$).

On en déduit :

$$
\mathbb{E}^{\mathbb{Q}}\left[
e^{\sigma\sqrt{T} Z} \mathbf{1}_{{Z>z^*}}
\right]
=
e^{\frac{1}{2}\sigma^2 T}
\int_{z^* - \sigma\sqrt{T}}^{\infty} \varphi(u), du
=
e^{\frac{1}{2}\sigma^2 T}
N!\bigl(- (z^* - \sigma\sqrt{T})\bigr).
\tag{10.11}
$$

En utilisant ($N(-x) = 1 - N(x)$) si besoin, mais surtout, on va définir directement :

$$
d_1 := -z^* + \sigma\sqrt{T}.
\tag{10.12}
$$

Alors :

$$
\mathbb{E}^{\mathbb{Q}}\left[
e^{\sigma\sqrt{T} Z} \mathbf{1}_{{Z>z^*}}
\right]
=
e^{\frac{1}{2}\sigma^2 T} N(d_1).
\tag{10.13}
$$

En combinant avec (10.8) :

$$
\mathbb{E}^{\mathbb{Q}}[S_T \mathbf{1}_{{Z>z^*}}]
=
S_0 e^{\left(r - \frac{1}{2}\sigma^2\right)T}
\cdot
e^{\frac{1}{2}\sigma^2 T} N(d_1)
=
S_0 e^{rT} N(d_1).
\tag{10.14}
$$

---

## 8.3 Substitution dans le prix du call et identification de (d_1), (d_2)

On revient maintenant à (10.6) :

$$
C_0
=
e^{-rT}
\left(
\mathbb{E}^{\mathbb{Q}}[S_T \mathbf{1}_{{Z>z^*}}]
-
K, \mathbb{P}^{\mathbb{Q}}(Z>z^*)
\right)
=
e^{-rT}
\left(
S_0 e^{rT} N(d_1)
-
K N(-z^*)
\right).
$$

Donc :

$$
C_0 = S_0 N(d_1) - K e^{-rT} N(-z^*).
\tag{10.15}
$$

On définit alors :

$$
d_2 := -z^*.
\tag{10.16}
$$

Ainsi :

$$
C_0 = S_0 N(d_1) - K e^{-rT} N(d_2).
\tag{10.17}
$$

Il reste à exprimer explicitement ($d_1$) et ($d_2$) en fonction des paramètres du modèle.

---

## 8.4 Expressions explicites de ($d_1$) et ($d_2$)

On repart de la définition de ($z^*$) (10.4) :

$$
z^* = \frac{\ln\left(\frac{K}{S_0}\right) - \left(r - \frac{1}{2}\sigma^2\right)T}{\sigma \sqrt{T}}.
$$

Donc :

$$
d_2 = -z^*
=
# \frac{-\ln\left(\frac{K}{S_0}\right) + \left(r - \frac{1}{2}\sigma^2\right)T}{\sigma \sqrt{T}}

\frac{\ln\left(\frac{S_0}{K}\right) + \left(r - \frac{1}{2}\sigma^2\right)T}{\sigma \sqrt{T}}.
\tag{10.18}
$$

Puis, d’après (10.12) :

$$
d_1 = -z^* + \sigma\sqrt{T}
= d_2 + \sigma\sqrt{T}.
\tag{10.19}
$$

Donc, explicitement :

$$
\boxed{
d_1 = \frac{\ln\left(\frac{S_0}{K}\right) + \left(r + \frac{1}{2}\sigma^2\right)T}{\sigma \sqrt{T}},
\qquad
d_2 = \frac{\ln\left(\frac{S_0}{K}\right) + \left(r - \frac{1}{2}\sigma^2\right)T}{\sigma \sqrt{T}}.
}
\tag{10.20}
$$

On vérifie bien que :

$$
d_1 = d_2 + \sigma\sqrt{T}.
$$

---

## 8.5 Formule de Black–Scholes pour un call européen

Nous avons ainsi obtenu la formule fermée du call européen (modèle Black–Scholes sans dividendes) :

$$
\boxed{
C_0 = S_0 N(d_1) - K e^{-rT} N(d_2),
}
\tag{10.21}
$$

avec ($d_1, d_2$) définis par (10.20).

Cette formule découle **uniquement** de :

1. la dynamique log-normale sous la mesure risque-neutre,
2. l’intégration explicite du payoff ($(S_T - K)^+$) via un changement de variable normal,
3. la manipulation de l’intégrale gaussienne par complétion de carré.

Dans une version avec dividende continu de taux ($q$), on remplace simplement :

* ($S_0$) par ($S_0 e^{-qT}$) dans la première espérance,
* ce qui conduit à la formule classique :

$$
C_0 = S_0 e^{-qT} N(d_1) - K e^{-rT} N(d_2),
$$

avec ($d_1, d_2$) où ($r$) est remplacé par ($r - q$) dans les numérateurs.

## 9. Put-Call Parity

On considère :

* un sous-jacent ($S$) ne versant pas de dividende,
* une option européenne call ($C_0$) de strike ($K$), maturité ($T$),
* une option européenne put ($P_0$) de même strike et maturité,
* un actif sans risque ($B_t = e^{rt}$), taux ($r$) constant.

La parité put–call établit une relation **de non-arbitrage** entre les prix du call, du put, du sous-jacent et du zéro-coupon.

---

### 9.1 Énoncé de la parité put–call

En absence d’arbitrage, on a :

$$
\boxed{
C_0 - P_0 = S_0 - K e^{-rT}.
}
$$

ou, de manière équivalente,

$$
\boxed{
C_0 + K e^{-rT} = P_0 + S_0.
}
$$

Les deux portefeuilles :

* **Portefeuille A** : long 1 call, short 1 put,
* **Portefeuille B** : long 1 action, short 1 zéro-coupon de nominal ($K$) à ($T$),

ont le **même payoff en ($T$)** dans tous les scénarios. La parité impose que leurs prix initiaux doivent coïncider, faute de quoi il existe une stratégie d’arbitrage statique.

---

### 9.2 Preuve par comparaison de payoffs

Considérons les deux portefeuilles suivants :

* ($A_0 = C_0 - P_0$) : long 1 call, short 1 put (même ($(K,T)$))
* ($B_0 = S_0 - K e^{-rT}$) : long 1 sous-jacent, short 1 zéro-coupon de nominal ($K$)

Regardons leurs valeurs à la maturité ($T$), pour un niveau quelconque ($S_T$).

#### Portefeuille A en ($T$)

Le payoff du call est ($(S_T - K)^+$), celui du put ($(K - S_T)^+$).

$$
A_T = (S_T - K)^+ - (K - S_T)^+.
$$

Or on a l’identité élémentaire valable pour tout ($x\in\mathbb{R}$) :

$$
(x - K)^+ - (K - x)^+ = x - K.
$$

En l’appliquant avec ($x = S_T$), on obtient :

$$
A_T = S_T - K.
$$

#### Portefeuille B en ($T$)

* L’action vaut ($S_T$).
* Le zéro-coupon de nominal ($K$) vaut ($K$).

Donc :

$$
B_T = S_T - K.
$$

Les deux portefeuilles ont **strictement le même payoff** dans tous les états du monde.

---

### 9.3 Argument de non-arbitrage

Si, au temps ($0$), on avait ($A_0 \neq B_0$), il serait possible de construire une stratégie d’arbitrage :

* Si ($A_0 < B_0$) :

  * acheter (A),
  * vendre (B),
  * gain certain ($B_0 - A_0 > 0$) à ($t=0$),
  * pas de risque en ($T$) car ($A_T = B_T$).

* Si ($A_0 > B_0$) :

  * vendre (A),
  * acheter (B),
  * même raisonnement.

L’absence d’arbitrage impose donc :

$$
A_0 = B_0,
$$

c’est-à-dire :

$$
C_0 - P_0 = S_0 - K e^{-rT}.
$$

---

### 9.4 Version avec dividende continu

Si le sous-jacent verse un dividende continu de taux ($q$), la parité devient :

$$
C_0 - P_0 = S_0 e^{-qT} - K e^{-rT}.
$$

La seule modification concerne la valeur actuelle du sous-jacent “forwardisé” via le dividende.

---

### 9.5 Comment utiliser la parité dans la suite

La parité put–call est utilisée :

* pour déduire la formule du put à partir de celle du call,
* pour contrôler la cohérence des prix observés sur le marché,
* pour reconstruire une jambe (call ou put) à partir de l’autre,
* pour définir certaines stratégies structurées de base.

Elle est **indépendante** du modèle Black–Scholes lui-même :
elle repose uniquement sur la structure de payoff, l’absence d’exercice anticipé (européen) et l’absence d’arbitrage.

## 14. Distribution du Sous-Jacent en Maturité

## 15. Calcul des Greeks
### 15.1 Delta  
### 15.2 Gamma  
### 15.3 Vega  
### 15.4 Theta  
### 15.5 Rho  

## 16. Sensibilités au Smile / Skew (Notions Préliminaires)

## 17. Interprétation Probabiliste (Feynman-Kac)

## 18. Limites du Modèle Black-Scholes

## 19. Extensions Naturelles
### 19.1 Dividendes  
### 19.2 Volatilité Locale  
### 19.3 Volatilité Stochastique (Heston)  
### 19.4 Modèles de Diffusion Normale (Bachelier)

## 20. Conclusion
