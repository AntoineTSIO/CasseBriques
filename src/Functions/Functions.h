//
// Created by Nicolas Pierrot on 26/10/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Structs.h"
#include "../AboutItem/AboutItem.h"

#ifndef CASSEBRIQUES_FUNCTIONS_H
#define CASSEBRIQUES_FUNCTIONS_H

int chooseGameType();
Player **createPlayers(int numberOfPlayersWished, int numberOfHumanPlayers);
void deletePlayers(Player **players, int nbPlayersWished);
Game* initGame();
void displayGameStats(Game* game);
void displayMapStats(Game* game);
void displayPlayerStats(Game* game);
Map procedurallyInitMap(Game *game, short isRandomlyDefined);
void displayMap(Game *game);
void deleteTile(Tile *tile);
void deleteMap(Game *game);
void deleteGame(Game *game);
char keypress();
void clearScreen();
void bombKick(Game *game, short direction);
short getPlayerAction();
void botAction (Game* game);
void playerAction(Game *game);
void hitPlayer(Player *dommageCollateral, Game *game);
void nextMap(Game* game);
Map initMapFromFile(Game *game, char *mapName);

#endif // CASSEBRIQUES_FUNCTIONS_H
