//
// Created by Nicolas Pierrot on 26/10/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Structs.h"
#include "AboutItem.h"


#ifndef CASSEBRIQUES_FUNCTIONS_H
#define CASSEBRIQUES_FUNCTIONS_H

Player **createPlayers(int nbPlayersWished) {
    // Création des players
    Player **players = malloc(nbPlayersWished * sizeof(Player));
    short idPlayers = 1;
    for (int indexNbPlayers = 0; indexNbPlayers < nbPlayersWished; ++indexNbPlayers) {
        Player *aPlayer = malloc(sizeof(Player));
        aPlayer->id = idPlayers;
        aPlayer->nbBomb = 1;
        aPlayer->range = 1;
        aPlayer->life = 3;
        aPlayer->shield = 0;
        aPlayer->passBomb = 0;
        aPlayer->invincibility = 0;
        aPlayer->nbKill = 0;
        players[indexNbPlayers] = aPlayer;
        idPlayers++;
    }
    return players;
}

void deletePlayers(Player **players, int nbPlayersWished) {
    for (int indexNbPlayers = 0; indexNbPlayers < nbPlayersWished; ++indexNbPlayers) {
        free(players[indexNbPlayers]);
    }
    free(players);
}

Game initGame() {
    Game game;
    printf("Initialisation du jeu\n");
    game.numberOfPlayers = 0;
    game.multiplayer = 0;
    game.players = NULL;
    game.nbBombsPerPlayer = 0;
    game.sizeMapX = 0;
    game.sizeMapY = 0;
    game.playerTurn = 0;

    while (game.sizeMapX < 10) {
        printf("Saisir la largeur de la carte :");
        scanf(" %hd", &game.sizeMapX);
        getchar();
    }
    while (game.sizeMapY < 10) {
        printf("Saisir la hauteur de la carte :");
        scanf(" %hd", &game.sizeMapY);
        getchar();
    }
    while (game.nbBombsPerPlayer < 1) {
        printf("Saisir le nombre de bombes par joueur :");
        scanf(" %hd", &game.nbBombsPerPlayer);
        getchar();
    }
    while (game.numberOfPlayers <= 0 || game.numberOfPlayers > 4) {
        printf("Saisir le nombre de joueurs :");
        scanf(" %hd", &game.numberOfPlayers);
        getchar();
        if (game.numberOfPlayers <= 0 || game.numberOfPlayers > 4) {
            printf("Le nombre de joueurs doit être compris entre 1 et 4\n");
        }
        if (game.numberOfPlayers > 1) {
            game.multiplayer = 1;
        }
    }
    game.players = *createPlayers(game.numberOfPlayers);
    return game;
}

void deleteGame(Game game) {
    deletePlayers(&game.players, game.numberOfPlayers);
}

void displayStats(Game game) {
    printf("Paramètres de jeu:\n");
    printf("Nombre de joueurs : %d\n", game.numberOfPlayers);
    printf("Nombre de bombes par joueur : %d\n", game.nbBombsPerPlayer);
    if (game.multiplayer == 1)
        printf("Mode: multiplayer\n");
    else
        printf("Mode: solo\n");
    printf("Dimensions de la carte: %d x %d\n", game.sizeMapX, game.sizeMapY);
}

Map initMap(Game game) {
    /*
    x = indestructible wall
    m = destructible wall
    b = bomb
    p = spawn point
    e = empty
    */
    Map map;

    map.tile = malloc(game.sizeMapX * sizeof(Tile *));
    for (int i = 0; i < game.sizeMapX; i++) {
        map.tile[i] = malloc(game.sizeMapY * sizeof(Tile));
    }

    for (int i = 0; i < game.sizeMapX; i++) {
        for (int j = 0; j < game.sizeMapY; j++) {
            map.tile[i][j].sprite = 'e';
            map.tile[i][j].item = NOTHING;
            map.tile[i][j].bomb = 0;
        }
    }

    for (int i = 0; i < game.sizeMapX; i++) {
        map.tile[i][0].sprite = 'x';
        map.tile[i][game.sizeMapY - 1].sprite = 'x';
        map.tile[i][0].item = INDESTRUCTIBLE_WALL;
    }
    for (int j = 0; j < game.sizeMapY; j++) {
        map.tile[0][j].sprite = 'x';
        map.tile[game.sizeMapX - 1][j].sprite = 'x';
        map.tile[0][j].item = INDESTRUCTIBLE_WALL;
    }

    for (int i = 1; i < game.sizeMapX - 1; i++) {
        for (int j = 1; j < game.sizeMapY - 1; j++) {
            if (i % 2 == 0 && j % 2 == 0) {
                map.tile[i][j].sprite = 'x';
                map.tile[i][j].item = INDESTRUCTIBLE_WALL;
            }
        }
    }

    for (int i = 0; i < game.numberOfPlayers-1; i++) {
        map.tile[1 + i * 2][1].sprite = 'p';
        map.tile[1 + i * 2][game.sizeMapY - 2].sprite = 'p';
        map.tile[1][1 + i * 2].item = NOTHING;
    }

    for (int i = 1; i < game.sizeMapX - 1; i++) {
        for (int j = 1; j < game.sizeMapY - 1; j++) {
            if (map.tile[i][j].sprite != 'x' && map.tile[i][j].sprite != 'p') {
                map.tile[i][j].sprite = 'm';
                map.tile[i][j].item = WALL;
            }
        }
    }

    return map;
}

void displayMap(Map map, Game game) {
// x = █
// m = ▒
// p =
    for (int i = 0; i < game.sizeMapX; ++i) {
        for (int j = 0; j < game.sizeMapY; ++j) {
            if (map.tile[i][j].sprite == 'x') {
                printf("█");
            } else if (map.tile[i][j].sprite == 'm') {
                printf("▒");
            } else if (map.tile[i][j].sprite == 'p') {
                printf("p");
            } else if (map.tile[i][j].sprite == 'b') {
                printf("b");
            } else if (map.tile[i][j].sprite == 'e' || map.tile[i][j].sprite == '_') {
                printf(" ");
            }
        }
        printf("\n");
    }
}


// void boom(Bomb *bombToExplode, Game game, Map *map) {
//     int i = 1;
//     while (i <= bombToExplode->range && (bombToExplode->x + i) < game.sizeMapX) {
//         short current_item = map->tile[bombToExplode->x + i][bombToExplode->y].item;
//         switch (current_item) {
//             case INDESTRUCTIBLE_WALL:
//                 break;
//             case DESTRUCTIBLE_WALL:
//                 current_item = spawn_random_item();
//                 break;
//             default:
//                 getting_pumped_up(game.players[game.playerTurn % 4], current_item);
//                 current_item = 0;
//                 break;
//         }
//     }
//     i = 1;
//     while (i <= bombToExplode->range && (bombToExplode->y + i) < game.sizeMapY) {

//     }
//     i = 1;
//     while (i <= bombToExplode->range && (bombToExplode->x - i) >= 0) {

//     }
//     i = 1;
//     while (i <= bombToExplode->range && (bombToExplode->x - i) >= 0) {

//     }
// }

//detect key pressed
int keypress() {
    system ("/bin/stty raw");
    int c;
    system ("/bin/stty -echo");
    c = getc(stdin);
    system ("/bin/stty echo");
    system ("/bin/stty cooked");

    /*
    z = 122 Z = 90
    q = 113 Q = 81
    s = 115 S = 83
    d = 100 D = 68
    e = 101 E = 69
    */
    return c;
}

#endif //CASSEBRIQUES_FUNCTIONS_H
