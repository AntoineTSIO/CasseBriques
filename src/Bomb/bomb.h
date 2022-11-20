#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Structs.h"
#include "../AboutItem/AboutItem.h"

#ifndef bomb_h
#define bomb_h

BombList *newBombNode(Bomb *bomb);

Bomb *newBomb(Game *game);

void setOffBombs(Game *game);

void boom(Bomb *bombToExplode, Game *game, short **boomMap);

#endif