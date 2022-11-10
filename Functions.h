//
// Created by Nicolas Pierrot on 26/10/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/lib/sdl2/2.24.2/include/SDL2/SDL.h"
#include "Structs.h"
#include "AboutItem.h"

#ifndef CASSEBRIQUES_FUNCTIONS_H
#define CASSEBRIQUES_FUNCTIONS_H

Player **createPlayers(int nbPlayersWished);
void deletePlayers(Player **players, int nbPlayersWished);
Game initGame();
void deleteGame(Game game);
void displayStats(Game game);
Map initMap(Game game);
void displayMap(Game game);
void deleteMap(Game game);
char keypress();
void clearScreen();
Game playerMovement(Game game);
Game spawnPlayers(Game game);



#endif //CASSEBRIQUES_FUNCTIONS_H
