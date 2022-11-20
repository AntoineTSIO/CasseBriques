# Casse Briques Groupe 7
[![Ubuntu](https://img.shields.io/badge/Ubuntu-22.04-E95420?style=flat-square&logo=ubuntu&logoColor=white)](https://releases.ubuntu.com/22.04/)
[![C](https://img.shields.io/badge/c-11-%2300599C.svg?style=flat-square&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
## Présentation du jeu

“Casse Briques” est un jeu dont le but est d’exploser ses ennemis et de gagner la bataille en
restant le dernier en vie. Le plateau de jeu comporte des murs destructibles et d’autres
indestructibles.

Les personnages peuvent se déplacer dans les espaces libres et déposer des bombes dans le
but de détruire des murs ou d’exploser leurs adversaires. Certains objets aléatoires peuvent
apparaître lors de la destruction d’un mur et ainsi augmenter la portée de la bombe, le nombre
de bombes possédées, etc...

## Documentation

* [Sujet du Casse Briques](/docs/Sujet_Casse_Briques.pdf)

## Installation

Pour que l'affichage graphique puisse fonctionner, il est nécessaire d'installer le package Dev de GTK 3 avec la commande suivante : 
```bash
sudo apt-get install libgtk-3-dev
```
Une fois cela fait, vous avez juste à vous rendre dans le projet avec la commande :
```bash
cd CasseBriques/
```
Enfin, la dernière étape pour lancer le jeu est uniquement de suivre le [Makefile](#makefile)

## Membres du groupe

- [Gabriel BONJOUR](https://github.com/Elbub)
- [Antoine TOUZART](https://github.com/AntoineTSIO)
- [Nicolas PIERROT](https://github.com/ncls-p)

## Table des matières

- [Casse Briques Groupe 7](#casse-briques-groupe-7)
  - [Présentation du jeu](#présentation-du-jeu)
  - [Documentation](#documentation)
  - [Installation](#installation)
  - [Membres du groupe](#membres-du-groupe)
  - [Table des matières](#table-des-matières)
  - [Makefile](#makefile)
    - [Compiler Nettoyer Jouer](#compiler-nettoyer-jouer)
    - [Compilation](#compilation)
    - [Nettoyage](#nettoyage)
    - [Jouer](#jouer)
    - [voir les commandes disponibles](#voir-les-commandes-disponibles)
  - [Journal](#journal)
    - [16 Octobre](#16-octobre)
    - [24 Octobre](#24-octobre)
    - [26 Octobre](#26-octobre)
    - [29 Octobre](#29-octobre)
    - [30 Octobre](#30-octobre)
    - [02 Novembre](#02-novembre)
    - [05 Novembre](#05-novembre)
    - [07 Novembre](#07-novembre)
    - [08 Novembre](#08-novembre)
    - [09 Novembre](#09-novembre)
    - [11 Novembre](#11-novembre)
    - [16 Novembre](#16-novembre)
    - [19 Novembre](#19-novembre)
    - [20 Novembre](#20-novembre)
  - [Licence](#licence)
- [](#)
>>>>>>> d438748 (feat: Update README.md)

## Makefile

### Compiler Nettoyer Jouer

Exécutez la commande suivante pour compiler, nettoyer et jouer au jeu.

`make run`

### Compilation

Exécutez la commande suivante pour compiler le projet :

`make build`

### Nettoyage

Exécutez la commande suivante pour nettoyer le projet :

`make clean`

### Jouer

Exécutez la commande suivante pour jouer au jeu :

`make play`

### voir les commandes disponibles

Exécutez la commande suivante pour voir les commandes disponibles :

`make help`

## Journal

### 16 Octobre

- Mise en place du projet sur **github**
- Mise en place d'un pc ubuntu pour le développement via **VSCode** et **LiveShare**
- Répartition des tâches de base entre les membres du groupe
- Départ du développement de la structure du projet
- Création du fichier **README.md** pour la documentation du projet
- Création du fichier **AVANCEE.md** pour la documentation de l'avancée du projet (**_tâches en cours, tâches
  prochaines, structures/fonctions pour chaque membre du groupe_**)

### 24 Octobre

- Mise en place de la création des joueurs
- Update du fichier **README.md** pour la documentation du projet
- Update du fichier **AVANCEE.md** pour la documentation de l'avancée du projet (**_tâches en cours, tâches prochaines,
  structures/fonctions pour chaque membre du groupe_**)

### 26 Octobre

- Création d'un fichier header pour les fonctions
- Déplacement des fonctions dans un fichier header
- Jointure des fichier header de Structs & de fonctions afin de les utiliser dans la classe Main

### 29 Octobre

- Création de la fonction d'initialisation de la partie
- Création du fichier Makefile

### 30 Octobre
- Début de la création de la map
- Début de la création de l'affichage de la map
- refonte de structs
- mise à jour du fichier README.md

### 02 Novembre
- Répartition de nouvelles tâches de base entre les membres du groupe
- Modifications sur le fichier AVANCEE.md
- Mise à jour du fichier README.md

### 05 Novembre
- Très gros Refactoring du code
- mise au propre du code
- Fusion du travail
- mise en place de nom de variable plus explicite
- mise à jour du fichier README.md
- ajout de la détection de touche du clavier
- update Tile status on map initialisation

### 07 Novembre
- Réparation de la création de la map
- Création de la map finalisée
- Refactoring du code pour séparation entre headers et sources
- réajustement des spawns des joueurs
- Ajout des joueurs sur la map à l'emplacement des spawns
- création de la fonction de déplacement des joueurs
- mise à jour du fichier README.md pour le journal

### 08 Novembre
- Modification Makefile -> ajout SDL
- Ajout de la librairie SDL
- Début de l'utilisation de SDL

### 09 Novembre
- Ajout des fichiers de la librairie SDL directement dans le projet
- Début de l'utilisation de SDL
- Mise à jour du README.md

### 11 Novembre
- Refonte des fonctions d'action des joueurs
- Modification des structures pour y correspondre
- Modification des fonctions qui utilisent les structures mises à jour (génération, suppresion, affichage)

### 16 Novembre
- Suppression des fichiers de la librairie SDL inclus directement dans le projet
- Passage sur une VM Ubuntu 22.04
- Début de l'utilisation de SDL
- Mise à jour du README.md
- Point sur l'avancée du projet
- Répartition des dernières taches
- Ajout d'une deadline supplémentaire pour la mise en place de SDL (17/11/22)

### 19 Novembre
- Refractor du code
- Debug du code
- Début implémentation socket

### 20 Novembre
- Refractor du code
- Debug du code
- Suite implémentation socket
- Mise à jour du README
- Ajout du sujet dans le dossier docs

## Licence
Voir le fichier [LICENSE.md](LICENSE.md) fourni.


#
[(Back to top)](#casse-briques-groupe-7)
