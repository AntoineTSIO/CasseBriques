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

Map initMap(Game game){
    // x = indestructible wall
    //m = destructible wall
    //b = bomb
    //p = spawn point
    //e = empty

    Map map;
    map.cases = malloc(game.dimensionsCarteX * sizeof(char*));
    for (int i = 0; i < game.dimensionsCarteX; ++i) {
        map.cases[i] = malloc(game.dimensionsCarteY * sizeof(char));
    }


    for (int i = 0; i < game.dimensionsCarteX; ++i) {
        for (int j = 0; j < game.dimensionsCarteY; ++j) {
            map.cases[i][j] = 'e';
        }
    }
    for (int i = 0; i < game.dimensionsCarteX; ++i) {
        map.cases[i][0] = 'x';
        map.cases[i][game.dimensionsCarteY-1] = 'x';
    }
    for (int i = 0; i < game.dimensionsCarteY; ++i) {
        map.cases[0][i] = 'x';
        map.cases[game.dimensionsCarteX-1][i] = 'x';
    }
    for (int i = 0; i < game.dimensionsCarteX; ++i) {
        for (int j = 0; j < game.dimensionsCarteY; ++j) {
            if (map.cases[i][j] == 'e') {
                if (rand() % 100 < 20) {
                    map.cases[i][j] = 'm';
                }
            }
        }
    }
    for (int i = 0; i < game.nombreJoueurs; ++i) {
        int x = rand() % (game.dimensionsCarteX-2) + 1;
        int y = rand() % (game.dimensionsCarteY-2) + 1;
        while (map.cases[x][y] != 'e') {
            x = rand() % (game.dimensionsCarteX-2) + 1;
            y = rand() % (game.dimensionsCarteY-2) + 1;
        }
        map.cases[x][y] = 'p';
    }
    return map;
}
// x = █
// m = ▒
// p =
void displayMap(Map map, Game game){
    for (int i = 0; i < game.dimensionsCarteX; ++i) {
        for (int j = 0; j < game.dimensionsCarteY; ++j) {
            if (map.cases[i][j] == 'x') {
                printf("█");
            }
            else if (map.cases[i][j] == 'm') {
                printf("▒");
            }
            else if (map.cases[i][j] == 'p') {
                printf("p");
            }
            else if (map.cases[i][j] == 'b') {
                printf("b");
            }
            else if (map.cases[i][j] == 'e') {
                printf(" ");
            }
        }
        printf("\n");
    }
}


#endif //CASSEBRIQUES_FUNCTIONS_H
