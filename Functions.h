//
// Created by Nicolas Pierrot on 26/10/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Structs.h"


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

Game initGame(){
    Game game;
    printf("Initialisation du jeu\n");
    game.nombreJoueurs = 0;
    game.multijoueur = 0;
    game.Joueur = NULL;
    while(game.nombreJoueurs <= 0 || game.nombreJoueurs > 4){
        printf("Saisir le nombre de joueurs :");
        scanf(" %d", &game.nombreJoueurs);
        getchar();
        if(game.nombreJoueurs <= 0 || game.nombreJoueurs > 4){
            printf("Le nombre de joueurs doit être compris entre 1 et 4\n");
        }
        if(game.nombreJoueurs > 1){
            game.multijoueur = 1;
        }
    }
    // Création des joueurs et ajout dans la partie
    game.Joueur = createPlayers(game.nombreJoueurs);
    return game;
}

void deleteGame(Game game){
    deletePlayers(game.Joueur, game.nombreJoueurs);
}

void displayStats(Game game){
    printf("Affichage des stats\n");
    printf("Nombre de joueurs : %d\n", game.nombreJoueurs);
    if (game.multijoueur == 1)
        printf("Mode multijoueur\n");
    else
        printf("Mode solo\n");
}

#endif //CASSEBRIQUES_FUNCTIONS_H
