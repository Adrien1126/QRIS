# PHASE 0 — QUESTIONNEMENT STRUCTURÉ

## Projet moteur de taux — *sans aucune solution imposée*

---

## 🧭 BLOC 1 — Intention du projet (à clarifier AVANT tout)

1. **Quel est le problème précis que je veux résoudre en v0.1 ?**
   (pas “un moteur de taux”, mais *quoi exactement* sait-il faire)
   L'objectif de créer un moteur de taux est de développer mes compétences sur les marchés de taux, tout en se basant sur la littérature afin de le rendre évolutif. Dans un premier temps, le projet se concentrera sur le pricing de base par actualisation au début (dépot, FRA, swap) avec uniquement le marché de l'EURO et donc un discounting ESTR. De ce fait, les conventions seront pour commencer les conventions de base de la zone Euro (Act/360, ModifiedFollowing). Etant donnée la situation, il faudra aussi prendre en compte les échéanciers selon des conventions (Début Court, Début Long, Fin Long, Fin court) et donc définir un échéancier. 

   Le marché optionnel (cap/floor) sera traité dans un second temps du fait de la nécessité de modèles stochastiques ou d'arbres binomiaux/trinomiaux pour prendre en compte le caractère optionnel. 

   On implémentera aussi des mesures de risques. 
 
2. **Qu’est-ce que ce projet n’est PAS ?**
   (lister explicitement ce que tu refuses d’implémenter à ce stade)

   Ce projet n'est pas un projet général mais reste un projet académique qui a pour objectif d'appliquer les meilleures pratiques en C++. Il a pour vocation d'être présenté à des recruteurs et de montrer la qualité aussi bien technique que fonctionnelle afin de prétendre à un poste de Quant même junior.  

3. **Si je devais expliquer ce projet en 30 secondes à un quant rates**,
   que dirais-je *sans* mentionner C++ ?
   L'objectif de mon projet est de mettre en place une plateforme de simulation des marchés de taux, sans nécessairement utilisé de données réelles afin d'améliorer mon expertise et mieux comprendre l'ensemble des instruments, des modèles (Avantages/Inconvénients) ainsi que les méthodes numériques utilisables et leurs implémentations. 

4. **Qu’est-ce que je veux démontrer techniquement**, indépendamment de la finance ?
   (architecture, ownership, testabilité, rigueur…)
Mon objectif est de montrer une architecture professionnel respectant les dernières versions de C++ 17 et 20 tout en apprenant à vraiment connaître les marchés de taux et les instruments financiers qui les composent. Car à la fin le projet évoluera possiblement vers d'autres marchés (En particulier Commodities qui diffère grandement)
---

## 🧱 BLOC 2 — Frontières conceptuelles (fondamental)

5. **Quels sont les grands concepts métier irréductibles des marchés de taux ?**
   (pas des classes, des *concepts*)
   - Un ensemble d'instruments défini par un notionnel, un sous-jacent ou un taux fixe. 
   - Deposit, FRA, Swap, CrossCurrencySwap, DefaultSwap 
   - Cadre multicourbe
        - Courbes de taux forward pour projection des taux 
        - Courbes de discounting pour l'actualisation des fluxs 

   - Pricing par absence d'opportunités d'arbitrage 
   - Changement de numéraire, forward measure, swap measure, real measure, risk neutral measure. 
   - One factor short rate model (Vasicek, CIR,Black Karasinski, Extended Vasicek, HW)
   - Two factor short rate models 
   - Market models (Libor and Swap market models)
   - Calibration des modèles par rapport à des données de marchés. 
   - Monte-Carlo
   - Volatility smile and skew
   - Produits indexés sur l'inflation
   - Risque de contrepartie (XVA)
        

6. **Lesquels existent indépendamment de tout modèle de pricing ?**

- Les courbes de taux
- Les instruments

7. **Lesquels dépendent du marché, et lesquels décrivent uniquement un contrat ?**

- Le principe de calibration dépend de la procédure de pricing
- La valeur actuelle d'un instrument dépend des données des courbes de taux
- Les instruments ne décrivent uniquement un contrat. 
- La gestion des conventions est indépendante des données de marché et appartiennent aux instruments. 

8. **Si je retirais toute notion de “prix”, qu’est-ce qui resterait valide ?**

👉 Cette question seule suffit souvent à révéler un mauvais design futur.
- Si l'on retire toute notion de prix, la gestion des échéanciers ne changerait pas, la définition des contrats non plus. 

A l'inverse, afin de calibrer les courbes de taux par bootstrapping, il est nécessaire d'avoir la notion de prix des contrats et pour effectuer le pricing, on a besoin des courbes de taux. 
---

## ⏱ BLOC 3 — Temps & conventions (le piège classique)

9. **Quelle est la représentation minimale du temps dont j’ai besoin ?**

- Afin de représenter les dates des contrats, on a besoin de pouvoir définir des dates. 
- De plus, on retrouve dans les contrats swaps par exemple, des périodicités de paiement et des périodicités de révision sur les taux flottants. On retrouve aussi la stub convention. 
- On a aussi la notion de calendrier selon la zone afin que les échéanciers ne tombent pas sur des jours de fermeture. 
- On recense aussi la convention au cas ou l'on tombe sur un jour férie (Modified Following, Following, Preceding)
- Et pour terminer, on a les conventions sur le fixing lag et le payment lag. 

10. **Le temps est-il une donnée, une règle, ou une convention ?**


11. **À quel moment une convention de day count intervient-elle vraiment ?**

* à la génération ?
* à l’actualisation ?
* ailleurs ?

- La day count convention peut être prise dès la génération des échéanciers car les échéanciers peuvent être édités à partir du moment où l'ensemble des conventions du contrat est donné. 

12. **Quelles incohérences temporelles mon moteur doit-il refuser par construction ?**

- Le moteur doit refuser qu'une date tombe un jour férié


## 💸 BLOC 4 — Cashflows (cœur réel des marchés de taux)

13. **Qu’est-ce qu’un cashflow, conceptuellement ?**

- Un cashflow correspond à un paiement, celui-ci peut être à taux fixe ou indexé sur un sous-jacent et donc la valeur d'un cashflow peut être déterminé une fois que le taux est observé. 

14. **Un cashflow connaît-il autre chose que sa date et son montant ?**


15. **Un cashflow peut-il exister sans marché ?**

- La définition même d'un cashflow n'a pas besoin de données de marché sauf dans le cas où celui-ci est à taux flottants car il est nécessaire d'avoir une courbe de taux forward pour déterminer son amount. Néanmoins, on peut possiblement considérer qu'un cashflow dépend d'un sous-jacent sans prendre en compte la nécessité de données de marché. 

16. **Qui est responsable de produire des cashflows :**

* le contrat ?
* un générateur ?
* un moteur ?

👉 Si tu hésites ici, note l’hésitation : elle est normale et révélatrice.

- La définition du contrat ne devrait pas produire les cashflows mais seulement en donner une définition 
- Un générateur pourrait donner la valeur des cashflows non actualisés 
- Un moteur prendrait en considération les courbes de taux forward et de discount afin d'actualiser le cashflow

---

## 🧾 BLOC 5 — Instruments (contrat vs calcul)

17. **Un instrument financier est-il un objet passif ou actif ?**

- Les données d'un contrat sont invariantes, donc on n'a un objet passif. 

18. **A-t-il le droit de connaître une courbe de taux ?**

- Le contrat ne connait pas les courbes de taux, la seule chose qui peut être connu est un string ou un enum qui indique le sous-jacent. mais le contrat ne connait pas d'un point de vue logiciel les courbes de taux, celles ci sont appelés lors du pricing. 

19. **A-t-il le droit de connaître une notion de “prix” ?**

- La seule notion de prix connu pour un contrat est le notionnel ou le taux du contrat. 

20. **Quelle est la responsabilité maximale acceptable d’un instrument ?**

La responsabilité maximale acceptable d'un instrument est de définir son échéancier de paiement et de fixing. 

---

## 💰 BLOC 6 — Pricing & actualisation

21. **Qu’est-ce que “pricer” signifie réellement dans un moteur de taux ?**

- Actualiser l'ensemble des fluxs et estimer la valeur actuelle net d'un contrat. 

22. **Actualiser = calcul mathématique ou décision métier ?**

- Calcul mathématique

23. **Qui décide *comment* on actualise ?**

* la courbe ?
* le moteur ?
* l’instrument ?

- Le moteur effectue les calculs d'actualisations et des taux forward car ce serait le seul lié aux courbes de taux. L'instrument indique juste l'index sous-jacent et l'index de la courbe de discount. 

24. **Puis-je changer la logique d’actualisation sans modifier les contrats ?**

- La logique d'actualisation peut être modifier sans modifier les contrats, mais l'actualisation se fait à parti d'un contrat et de courbes de taux 

---

## 🧠 BLOC 7 — Ownership & lifetime (C++ critique)

25. **Quels objets ont une durée de vie évidente et locale ?**

- Un instrument a une durée de vie locale, une fois que les données nécessaires ont été obtenus, celui-ci peut etre détruit. 
- Un modèle aussi, on définit un modèle pour effectuer le pricing d'un instrument. 

26. **Quels objets doivent survivre à plusieurs calculs ?**

- Néanmoins, les donnnées de marché doivent être partagés pour chaque contrat et ne doivent pas être détruite au risque que chaque contrat ne soit pas actualisés ou projetés avec les mêmes données. 

27. **Où une allocation dynamique est-elle inévitable ?**

- Dans le moteur de pricing, il est important de définir un ptr vers un instrument et un ptr vers un modèle ou vers des données de marchés. 

28. **Où une allocation dynamique serait une erreur conceptuelle ?**

29. **Si j’introduis un `unique_ptr`, qui possède réellement quoi ?**

Lorsqu'on introduit un unique_ptr, l'instance du pricer possède une référence vers un modèle et un instrument de manière unique. Dans le cas, où l'on appelle des données de marché, on utilisera un shared_ptr qui sera plus adapté afin que tous les instruments puissent être valoriser à partir des mêmes données. 
👉 Si tu ne peux pas répondre clairement → n’introduis pas de pointeur.

---

## 🧪 BLOC 8 — Tests (penser AVANT de coder)

30. **Quels composants doivent être testables sans aucun pricing ?**

- Les instruments, les modèles, les données de marché ainsi que les dates, les schedules sont purement déterministes. 

31. **Quels bugs seraient inacceptables dans un moteur de taux ?**

32. **Quels résultats doivent être strictement déterministes ?**

- Les résultats par actualisation via courbe de taux sont purement déterministes. 
Tandis que ceux qui dépendent d'un modèle probabiliste ne le sont pas. 

33. **Que dois-je être capable de tester avant même d’avoir une courbe ?**

- Avant d'avoir une courbe, on doit être en mesure de gérer la gestion des dates, la création des instruments. 
- La calibration des données de marché peut aussi etre tester sans meme avoir une courbe. 

---

## 🔄 BLOC 9 — Évolutivité (toi dans 6–12 mois)

34. **Si j’ajoute une floating leg, qu’est-ce qui casse ?**

35. **Si j’ajoute un bootstrap, où ira la complexité ?**

36. **Quelle partie du système doit rester quasiment figée ?**

37. **Quelle partie est supposée évoluer constamment ?**

---

## 🧾 BLOC 10 — Discipline d’ingénierie (très important)

38. **Quelles dépendances dois-je interdire par principe ?**

39. **Qu’est-ce qui ne doit JAMAIS dépendre du pricing ?**

40. **Si je supprime un module entier, lequel ne doit rien remarquer ?**

