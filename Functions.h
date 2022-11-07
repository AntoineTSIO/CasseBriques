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

Player **createPlayers(int nbPlayersWished);
void deletePlayers(Player **players, int nbPlayersWished);
Game initGame();
void deleteGame(Game game);
void displayStats(Game game);
Map initMap(Game game);
void displayMap(Game game);
void deleteMap(Game game);
int keypress();
void clearScreen();



#endif //CASSEBRIQUES_FUNCTIONS_H
