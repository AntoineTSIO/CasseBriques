# Avancée du Casse Briques


## Taches en cours
- Créations des fichiers en .h pour faire les structures
// Nicolas

- Gestion des explosions
//Gabriel

- Gestion des déplacements
//Gabriel

- Gestion des objets
//Gabriel

- Gestion de la carte de base
//Antoine

- Menu et config de lancement d'une partie
//Nicolas

- Game over

- Gestion de l'affichage graphique (SDL)
//Antoine

---
## Taches prochaines

- gestion socket.io

- Gestion du/des easter egg(s)
    - Cuisson d'une omelette
    - maskedMap

---

## Strucutures / Fonctions


player{
   portée
   vie
   bouclier
   kick/pass bomb
   invincibilité
   nb_kills
}

bombe{
   propriétaire
   portée
   timer
   tronche
   x
   y
}

powerup{
   id
   tronche
}

tile{
   item
   bomb
   player
}