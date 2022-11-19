#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../Structs.h"


#ifndef CASSEBRIQUES_ABOUTITEM_H
#define CASSEBRIQUES_ABOUTITEM_H

// Items
#define INDESTRUCTIBLE_WALL -2
#define WALL -1
#define NOTHING 0
#define PROBABILITY_OF_NOTHING 1000
#define BOMB_UP 1
#define PROBABILITY_OF_BOMB_UP 150
#define BOMB_DOWN 2
#define PROBABILITY_OF_BOMB_DOWN 120
#define RANGE_UP 3
#define PROBABILITY_OF_RANGE_UP 225
#define RANGE_DOWN 4
#define PROBABILITY_OF_RANGE_DOWN 175
#define RANGE_AT_MAX 5
#define PROBABILITY_OF_RANGE_AT_MAX 10
#define BOMB_PASS 6
#define PROBABILITY_OF_BOMB_PASS 75
#define BOMB_KICK 7
#define PROBABILITY_OF_BOMB_KICK 75
#define INVINCIBILITY 8
#define PROBABILITY_OF_INVINCIBILITY 30
#define SHIELD 9
#define PROBABILITY_OF_SHIELD 50
#define LIFE_UP 10
#define PROBABILITY_OF_LIFE_UP 90
#define PLAYER 11
//#define DUCK 10000       // Eh, why not transform the player into a duck ?

// Movement
   // Direction
#define MOVEMENT_KEY_ERROR -1
#define DONT_MOVE 0
#define XPLUS 1
#define XMINUS 2
#define YPLUS 3
#define YMINUS 4
#define PUT_BOMB 5
   // Bomb interaction
#define PIETON 0
#define BOMB_WALK 1
#define BOMB_KICKING 2

short getRandomItem();
Item* newItem(short itemId);
void pickUpItem(Game *game, short whichOne);

/*
int player_moves(Game *game, short direction, Map *map);
*/
#endif