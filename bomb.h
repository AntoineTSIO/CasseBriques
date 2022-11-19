#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/lib/sdl2/2.24.2/include/SDL2/SDL.h"
#include "Structs.h"
#include "AboutItem.h"

#ifndef bomb_h
#define bomb_h

BombList* newBombNode(Bomb* bomb);

Bomb* createBomb(Game* game);

void setOffBombs(Game* game);

void boom (Bomb* bombToExplode, Game* game, short** boomMap);

#endif