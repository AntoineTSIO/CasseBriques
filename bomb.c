#include <stdio.h>
#include <stdlib.h>
#include "bomb.h"
#include "AboutItem.h"
#include "Functions.h"


BombList* newBombNode(Bomb* bomb){
   BombList* newNode = malloc(sizeof(BombList));
   newNode->thisBomb = bomb;
   newNode->nextOne = NULL;
   return newNode;
}

Bomb* createBomb(Game* game){
   Bomb* bomb = malloc(sizeof(Bomb));
   bomb->range = game->currentPlayer->range;
   bomb->x = game->currentPlayer->x;
   bomb->y = game->currentPlayer->y;
   bomb->timer = game->numberOfPlayers * 3 + 1;
   bomb->sprite = 'b';
   bomb->owner = game->currentPlayer;
   BombList* bombNode = newBombNode(bomb);
   if (game->activeBombs == NULL){
      game->activeBombs = bombNode;
   }else{
      BombList* nextBombInList = game->activeBombs;
      while (nextBombInList->nextOne != NULL){ 
         nextBombInList = nextBombInList->nextOne;
      }
      nextBombInList->nextOne = bombNode;
   }
   return bomb;
}

void boom (Bomb* bombToExplode, Game* game, short** boomMap){
   boomMap[bombToExplode->x][bombToExplode->y] = 1;
    Map* map = &game->map[game->currentMap];
   // The 4 next loops are just one done for each direction.
   // The flame of the explosion goes up to one wall (and destructs it if able) or up to range, whichever the less.
   // It sets off other bombs it touches and destroys all items in its path.
   int i = 1;
   while (i <= bombToExplode->range){
      Item* currentItem = map->tile[(bombToExplode->x + i) % map->sizeMapX][bombToExplode->y].whichItemIsHere;
      if (boomMap[(bombToExplode->x + i) % map->sizeMapX][bombToExplode->y] == 1
          && !(currentItem != NULL && currentItem->ID == WALL)){
         i++;
         continue;
      }
      if (map->tile[(bombToExplode->x + i) % map->sizeMapX][bombToExplode->y].whichBombIsHere != NULL){
         boom (map->tile[(bombToExplode->x + i) % map->sizeMapX][bombToExplode->y].whichBombIsHere, game, boomMap);
      }else{
      if (currentItem != NULL && currentItem->ID == INDESTRUCTIBLE_WALL)
         break;
      boomMap[(bombToExplode->x + i) % map->sizeMapX][bombToExplode->y] = 1;
      if (currentItem != NULL && currentItem->ID == WALL)
         break;
      }
      i++;
   }

   i = 1;
   while (i <= bombToExplode->range){
      Item* currentItem = map->tile[bombToExplode->x][(bombToExplode->y + i) % map->sizeMapY].whichItemIsHere;
      if (boomMap[bombToExplode->x][(bombToExplode->y + i) % map->sizeMapY] == 1
          && !(currentItem != NULL && currentItem->ID == WALL)){
         i++;
         continue;
      }
      if (map->tile[bombToExplode->x][(bombToExplode->y + i) % map->sizeMapY].whichBombIsHere != NULL){
         boom (map->tile[bombToExplode->x][(bombToExplode->y + i) % map->sizeMapY].whichBombIsHere, game, boomMap);
      }
      if (currentItem != NULL && currentItem->ID == INDESTRUCTIBLE_WALL)
         break;
      boomMap[bombToExplode->x][(bombToExplode->y + i) % map->sizeMapY] = 1;
      if (currentItem != NULL && currentItem->ID == WALL)
         break;
      i++;
   }

   i = 1;
   while (i <= bombToExplode->range){Item* currentItem = map->tile[(bombToExplode->x - i) % map->sizeMapX][bombToExplode->y].whichItemIsHere;
      if (boomMap[(bombToExplode->x - i) % map->sizeMapX][bombToExplode->y] == 1
          && !(currentItem != NULL && currentItem->ID == WALL)){
         i++;
         continue;
      }
      if (map->tile[(bombToExplode->x - i) % map->sizeMapX][bombToExplode->y].whichBombIsHere != NULL){
         boom (map->tile[(bombToExplode->x - i) % map->sizeMapX][bombToExplode->y].whichBombIsHere, game, boomMap);
      }
      if (currentItem != NULL && currentItem->ID == INDESTRUCTIBLE_WALL)
         break;
      boomMap[(bombToExplode->x - i) % map->sizeMapX][bombToExplode->y] = 1;
      if (currentItem != NULL && currentItem->ID == WALL)
         break;
      i++;
   }

   i = 1;
   while (i <= bombToExplode->range){
      Item* currentItem = map->tile[bombToExplode->x][(bombToExplode->y - i) % map->sizeMapY].whichItemIsHere;
      if (boomMap[bombToExplode->x][(bombToExplode->y - i) % map->sizeMapY] == 1
          && !(currentItem != NULL && currentItem->ID == WALL)){
         i++;
         continue;
      }
      if (map->tile[bombToExplode->x][(bombToExplode->y - i) % map->sizeMapY].whichBombIsHere != NULL){
         boom (map->tile[bombToExplode->x][(bombToExplode->y - i) % map->sizeMapY].whichBombIsHere, game, boomMap);
      }
      if (currentItem != NULL && currentItem->ID == INDESTRUCTIBLE_WALL)
         break;
      boomMap[bombToExplode->x + i][(bombToExplode->y - i) % map->sizeMapY] = 1;
      if (currentItem != NULL && currentItem->ID == WALL)
         break;
      i++;
   }

   bombToExplode->owner->numberOfBombsLeft++;

   // Free the bomb that just exploded.

   map->tile[bombToExplode->x][bombToExplode->y].whichBombIsHere = NULL;
   BombList* nextBombInList = game->activeBombs;
   if (nextBombInList->thisBomb->x == bombToExplode->x && nextBombInList->thisBomb->y == bombToExplode->y){
      game->activeBombs = nextBombInList->nextOne;
   }
   while (!(nextBombInList->nextOne->thisBomb->x == bombToExplode->x && nextBombInList->nextOne->thisBomb->y == bombToExplode->y)){
       // No need to check if NULL. This bomb can't not be in the list. Y'know, like checking if VIPs in the club are on the list.
      nextBombInList = nextBombInList->nextOne;
   }
   BombList* temp = nextBombInList->nextOne;
   nextBombInList->nextOne = nextBombInList->nextOne->nextOne;
   free(temp);
   free(bombToExplode);
}

void setOffBombs(Game* game){
    Map* map = &game->map[game->currentMap];
    BombList* bombToDowntime = game->activeBombs;
    while (bombToDowntime != NULL){
        bombToDowntime->thisBomb->timer--;
        bombToDowntime = bombToDowntime->nextOne;
    }

    short** boomMap = malloc(map->sizeMapX * sizeof(short *));
    for (int i = 0; i < map->sizeMapX; i++) {
        boomMap[i] = malloc(map->sizeMapY * sizeof(short));
        for (int j = 0; j < map->sizeMapY; j++){
            boomMap[i][j] = 0;
        }
    }

    // The boomMap has two uses : avoid item drop by walls to be destroyed the same turn they poped in and
    // keeping the player from taking multiple damage in one turn. It checks which tile will be cleansed by
    // fire, then it makes things actually go BOOM.
    for (int i = 0; i < map->sizeMapX; i++){
        for (int j = 0; j < map->sizeMapY; j++){
            if (boomMap[i][j] == 1){
                continue;
            }
            Bomb* currentTileBomb = map->tile[i][j].whichBombIsHere;
            if (currentTileBomb != NULL && currentTileBomb->timer <= 0){
                boom(currentTileBomb, game, boomMap);
            }
        }
    }

    // Display of explosions goes here

    for (int i = 0; i < map->sizeMapX; i++){
        for (int j = 0; j < map->sizeMapY; j++){
            if (boomMap[i][j] == 1){
                if(map->tile[i][j].whoIsHere != NULL){
                    hitPlayer(map->tile[i][j].whoIsHere, game);
                }
                if(map->tile[i][j].whichItemIsHere != NULL){
                    if(map->tile[i][j].whichItemIsHere->ID == WALL){
                        free(map->tile[i][j].whichItemIsHere);
                        map->tile[i][j].whichItemIsHere = newItem(getRandomItem());;
                    }else{
                        free(map->tile[i][j].whichItemIsHere);
                        map->tile[i][j].whichItemIsHere = NULL;
                    }
                }
            }
        }
    }
}

