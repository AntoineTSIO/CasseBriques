//
// Created by Nicolas Pierrot on 26/10/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Functions.h"

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
    game.teleportZone = 0;

    while (game.sizeMapX < 11) {
        printf("Saisir le numéro de carte :\n");
        printf("1 - 11x11\n");
        printf("2 - 11x11 + Zone de téléportation\n");
        printf("3 - 21x21\n");
        printf("4 - 21x21 + Zone de téléportation\n");
        printf("5 - 31x31\n");
        printf("6 - 31x31 + Zone de téléportation\n");
        printf("7 - 41x41\n");
        printf("8 - 41x41 + Zone de téléportation\n");

        char input[256];
        fgets(input, 256, stdin);
        int choice = atoi(input);

        switch (choice) {
            case 1:
                game.sizeMapX = 11;
                game.sizeMapY = 11;
                game.teleportZone = 0;
                break;
            case 2:
                game.sizeMapX = 11;
                game.sizeMapY = 11;
                game.teleportZone = 1;
                break;
            case 3:
                game.sizeMapX = 21;
                game.sizeMapY = 21;
                game.teleportZone = 0;
                break;
            case 4:
                game.sizeMapX = 21;
                game.sizeMapY = 21;
                game.teleportZone = 1;
                break;
            case 5:
                game.sizeMapX = 31;
                game.sizeMapY = 31;
                game.teleportZone = 0;
                break;
            case 6:
                game.sizeMapX = 31;
                game.sizeMapY = 31;
                game.teleportZone = 1;
                break;
            case 7:
                game.sizeMapX = 41;
                game.sizeMapY = 41;
                game.teleportZone = 0;
                break;
            case 8:
                game.sizeMapX = 41;
                game.sizeMapY = 41;
                game.teleportZone = 1;
                break;
            default:
                printf("Saisie incorrecte\n");
                break;
        }
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
        if (game.numberOfPlayers <= 0 || game.numberOfPlayers > 4)
            printf("Le nombre de joueurs doit être compris entre 1 et 4\n");
        if (game.numberOfPlayers > 1)
            game.multiplayer = 1;
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

    switch (game.numberOfPlayers) {
        case 1:
            map.tile[1][1].sprite = 'p';
            map.tile[1][1].item = NOTHING;
            break;
        case 2:
            map.tile[1][1].sprite = 'p';
            map.tile[1][1].item = NOTHING;
            map.tile[game.sizeMapX - 2][game.sizeMapY - 2].sprite = 'p';
            map.tile[game.sizeMapX - 2][game.sizeMapY - 2].item = NOTHING;
            break;
        case 3:
            map.tile[1][1].sprite = 'p';
            map.tile[1][1].item = NOTHING;
            map.tile[game.sizeMapX - 2][game.sizeMapY - 2].sprite = 'p';
            map.tile[game.sizeMapX - 2][game.sizeMapY - 2].item = NOTHING;
            map.tile[game.sizeMapX - 2][1].sprite = 'p';
            map.tile[game.sizeMapX - 2][1].item = NOTHING;
            break;
        case 4:
            map.tile[1][1].sprite = 'p';
            map.tile[1][1].item = NOTHING;
            map.tile[game.sizeMapX - 2][game.sizeMapY - 2].sprite = 'p';
            map.tile[game.sizeMapX - 2][game.sizeMapY - 2].item = NOTHING;
            map.tile[game.sizeMapX - 2][1].sprite = 'p';
            map.tile[game.sizeMapX - 2][1].item = NOTHING;
            map.tile[1][game.sizeMapY - 2].sprite = 'p';
            map.tile[1][game.sizeMapY - 2].item = NOTHING;

    }

    for (int i = 1; i < game.sizeMapX - 1; i++) {
        for (int j = 1; j < game.sizeMapY - 1; j++) {
            if (map.tile[i][j].sprite != 'x' && map.tile[i][j].sprite != 'p') {
                map.tile[i][j].sprite = 'm';
                map.tile[i][j].item = WALL;
            }
        }
    }

    for (int i = 1; i < game.sizeMapX - 1; i++) {
        for (int j = 1; j < game.sizeMapY - 1; j++) {
            if (map.tile[i][j].sprite == 'm') {
                if (map.tile[i - 1][j].sprite == 'p' || map.tile[i + 1][j].sprite == 'p' ||
                    map.tile[i][j - 1].sprite == 'p' || map.tile[i][j + 1].sprite == 'p') {
                    map.tile[i][j].sprite = 'e';
                    map.tile[i][j].item = NOTHING;
                }
            }
        }
    }

    if (game.teleportZone == 1) {
        map.tile[0][game.sizeMapY / 2].sprite = 'e';
        map.tile[0][game.sizeMapY / 2].item = NOTHING;
        map.tile[game.sizeMapX - 1][game.sizeMapY / 2].sprite = 'e';
        map.tile[game.sizeMapX - 1][game.sizeMapY / 2].item = NOTHING;
        map.tile[game.sizeMapX / 2][0].sprite = 'e';
        map.tile[game.sizeMapX / 2][0].item = NOTHING;
        map.tile[game.sizeMapX / 2][game.sizeMapY - 1].sprite = 'e';
        map.tile[game.sizeMapX / 2][game.sizeMapY - 1].item = NOTHING;

    }

    return map;
}

void displayMap(Game game) {
// x = █
// m = ▒
// p =
    for (int i = 0; i < game.sizeMapX; ++i) {
        for (int j = 0; j < game.sizeMapY; ++j) {
            if (game.map.tile[i][j].sprite == 'x') {
                printf("█");
            } else if (game.map.tile[i][j].sprite == 'm') {
                printf("▒");
            } else if (game.map.tile[i][j].sprite == 'p') {
                printf("p");
            } else if (game.map.tile[i][j].sprite == 'b') {
                printf("b");
            } else if (game.map.tile[i][j].sprite == 'e' || game.map.tile[i][j].sprite == '_') {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void deleteMap(Game game) {
    for (int i = 0; i < game.sizeMapX; i++) {
        free(game.map.tile[i]);
    }
    free(game.map.tile);
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
char keypress() {
    system("/bin/stty raw");
    int c;
    system("/bin/stty -echo");
    c = getc(stdin);
    system("/bin/stty echo");
    system("/bin/stty cooked");

    switch (c) {
        case 122:
            return 'z';
        case 90:
            return 'Z';
        case 113:
            return 'q';
        case 81:
            return 'Q';
        case 115:
            return 's';
        case 83:
            return 'S';
        case 100:
            return 'd';
        case 68:
            return 'D';
        case 101:
            return 'e';
        case 69:
            return 'E';
        default:
            return ' ';
    }
}

Game spawnPlayers(Game game){
    for(int i = 0; i < game.numberOfPlayers; i++){
        for(int j = 0; j < game.sizeMapX; j++){
            for(int k = 0; k < game.sizeMapY; k++){
                if(game.map.tile[j][k].sprite == 'p'){
                    game.players[i].x = j;
                    game.players[i].y = k;
                    game.map.tile[j][k].sprite = 'p';
                    break;
                }
            }
        }
    }
    return game;
}

void executeMovement(Tile **tile, Player player, int x, int y){
    if(tile[player.x + x][player.y + y].item == NOTHING){
        tile[player.x][player.y].sprite = 'e';
        tile[player.x + x][player.y + y].sprite = 'p';
        player.x += x;
        player.y += y;
    }
}

Game playerMovement(Game game) {
    Tile **tile = game.map.tile;
    Player player = game.players[game.playerTurn % 4];
    char key = keypress();
    switch (key) {
        case 'z':
            executeMovement(tile, player, -1, 0);
            break;
        case 'q':
            executeMovement(tile, player, 0, -1);
            break;
        case 's':
            executeMovement(tile, player, 1, 0);
            break;
        case 'd':
            executeMovement(tile, player, 0, 1);
            break;
    }
    game.players[game.playerTurn % 4] = player;
    return game;
}

void clearScreen() {
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}
