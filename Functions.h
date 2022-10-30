//
// Created by Nicolas Pierrot on 26/10/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"


#ifndef CASSEBRIQUES_FUNCTIONS_H
#define CASSEBRIQUES_FUNCTIONS_H

// int chooseNumberOfPlayers() {
//     // Séléction du nombre de joueurs
//     int nbJoueursSouhaites = 0;
//     do {
//         printf("Saisir le nombre de joueurs :");
//         scanf(" %d", &nbJoueursSouhaites);
//         getchar();
//     } while (nbJoueursSouhaites <= 0 || nbJoueursSouhaites > 4);

//     printf("\nNombre de joueurs : %d", nbJoueursSouhaites);
//     return nbJoueursSouhaites;
// }


Joueur **createPlayers(int nbJoueursSouhaites) {
    // Création des joueurs
    Joueur **joueurs = malloc(nbJoueursSouhaites * sizeof(Joueur));
    int idJoueur = 1;
    for (int indexNbJoueursSouhaites = 0; indexNbJoueursSouhaites < nbJoueursSouhaites; ++indexNbJoueursSouhaites) {
        Joueur *unJoueur = malloc(sizeof(Joueur));
        unJoueur->id = idJoueur;
        unJoueur->nbBombes = 1;
        unJoueur->portee = 1;
        unJoueur->vie = 3;
        unJoueur->bouclier = 0;
        unJoueur->passBomb = 0;
        unJoueur->invicibilite = 0;
        unJoueur->nbKills = 0;
        joueurs[indexNbJoueursSouhaites] = unJoueur;
        idJoueur++;
    }
    return joueurs;
}

void deletePlayers(Joueur **joueurs, int nbJoueursSouhaites) {
    for (int indexNbJoueursSouhaites = 0; indexNbJoueursSouhaites < nbJoueursSouhaites; ++indexNbJoueursSouhaites) {
        free(joueurs[indexNbJoueursSouhaites]);
    }
    free(joueurs);
}

Game initGame() {
    Game game;
    printf("Initialisation du jeu\n");
    game.nombreJoueurs = 0;
    game.multijoueur = 0;
    game.Joueurs = NULL;
    game.nbBombesParJoueur = 0;
    game.dimensionsCarteX = 0;
    game.dimensionsCarteY = 0;

    while (game.dimensionsCarteX < 10) {
        printf("Saisir la largeur de la carte :");
        scanf(" %d", &game.dimensionsCarteX);
        getchar();
    }
    while (game.dimensionsCarteY < 10) {
        printf("Saisir la hauteur de la carte :");
        scanf(" %d", &game.dimensionsCarteY);
        getchar();
    }
    while (game.nbBombesParJoueur < 1) {
        printf("Saisir le nombre de bombes par joueur :");
        scanf(" %d", &game.nbBombesParJoueur);
        getchar();
    }
    while (game.nombreJoueurs <= 0 || game.nombreJoueurs > 4) {
        printf("Saisir le nombre de joueurs :");
        scanf(" %d", &game.nombreJoueurs);
        getchar();
        if (game.nombreJoueurs <= 0 || game.nombreJoueurs > 4) {
            printf("Le nombre de joueurs doit être compris entre 1 et 4\n");
        }
        if (game.nombreJoueurs > 1) {
            game.multijoueur = 1;
        }
    }
    game.Joueurs = createPlayers(game.nombreJoueurs);
    return game;
}

void deleteGame(Game game) {
    deletePlayers(game.Joueurs, game.nombreJoueurs);
}

void displayStats(Game game) {
    printf("Paramètres de jeu:\n");
    printf("Nombre de joueurs : %d\n", game.nombreJoueurs);
    printf("Nombre de bombes par joueur : %d\n", game.nbBombesParJoueur);
    if (game.multijoueur == 1)
        printf("Mode: multijoueur\n");
    else
        printf("Mode: solo\n");
    printf("Dimensions de la carte: %d x %d\n", game.dimensionsCarteX, game.dimensionsCarteY);
}




#endif //CASSEBRIQUES_FUNCTIONS_H
