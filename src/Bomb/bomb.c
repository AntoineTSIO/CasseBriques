#include <stdio.h>
#include <stdlib.h>
#include "../../Structs.h"
#include "../AboutItem/AboutItem.h"
#include "../Functions/Functions.h"

BombList *newBombNode(Bomb *bomb){
   BombList *newNode = malloc(sizeof(BombList));
   newNode->thisBomb = bomb;
   newNode->nextOne = NULL;
   return newNode;
}

Bomb *newBomb(Game *game){
   Bomb *bomb = malloc(sizeof(Bomb));
   bomb->range = game->currentPlayer->range;
   bomb->x = game->currentPlayer->x;
   bomb->y = game->currentPlayer->y;
   bomb->timer = game->numberOfPlayers * 3 + 1;
   bomb->sprite = 'b';
   bomb->owner = game->currentPlayer;
   BombList *bombNode = newBombNode(bomb);
   if (game->activeBombs == NULL){
      game->activeBombs = bombNode;
   }
   else{
      BombList *nextBombInList = game->activeBombs;
      while (nextBombInList->nextOne != NULL){
         nextBombInList = nextBombInList->nextOne;
      }
      nextBombInList->nextOne = bombNode;
   }
   return bomb;
}

void displayBoomMap(Game *game, short** boomMap){
    displayPlayerStats(game);
    for (int i = 0; i < game->map[game->currentMap].sizeMapX; ++i){
        for (int j = 0; j < game->map[game->currentMap].sizeMapY; ++j){
            if (boomMap[i][j]){
                printf("F");
            }
            if (game->map[game->currentMap].tile[i][j].whichItemIsHere != NULL){
                printf("%c", game->map[game->currentMap].tile[i][j].whichItemIsHere->sprite);
            }
            else if (game->map[game->currentMap].tile[i][j].whoIsHere != NULL){ // Penser à afficher bombe ET joueurs présents sur la même case. Impossible avec juste des char, mais possible avec SDL.
                printf("%c", game->map[game->currentMap].tile[i][j].whoIsHere->sprite);
            }
            else if (game->map[game->currentMap].tile[i][j].whichBombIsHere != NULL){
                printf("%c", game->map[game->currentMap].tile[i][j].whichBombIsHere->sprite);
            }
            else
                printf(" ");
        }
        printf("\n");
    }
}

void boom(Bomb *bombToExplode, Game *game, short **boomMap){
   boomMap[bombToExplode->x][bombToExplode->y] = 1;
   Map *map = &game->map[game->currentMap];
   // The 4 next loops are just one done for each direction.
   // The flame of the explosion goes up to one wall (and destructs it if able) or up to range, whichever the less.
   // It sets off other bombs it touches and destroys all items in its path.
   int i = 1;
   while (i <= bombToExplode->range){
      Item *currentItem = map->tile[(bombToExplode->x + i) % map->sizeMapX][bombToExplode->y].whichItemIsHere;
      if (boomMap[(bombToExplode->x + i) % map->sizeMapX][bombToExplode->y] == 1 && !(currentItem != NULL && (currentItem->ID == WALL || currentItem->ID == INDESTRUCTIBLE_WALL))){
         i++;
         continue;
      }
      if (map->tile[(bombToExplode->x + i) % map->sizeMapX][bombToExplode->y].whichBombIsHere != NULL){
         boom(map->tile[(bombToExplode->x + i) % map->sizeMapX][bombToExplode->y].whichBombIsHere, game, boomMap);
      }
      else{
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
      Item *currentItem = map->tile[bombToExplode->x][(bombToExplode->y + i) % map->sizeMapY].whichItemIsHere;
      if (boomMap[bombToExplode->x][(bombToExplode->y + i) % map->sizeMapY] == 1 && !(currentItem != NULL && (currentItem->ID == WALL || currentItem->ID == INDESTRUCTIBLE_WALL))){
         i++;
         continue;
      }
      if (map->tile[bombToExplode->x][(bombToExplode->y + i) % map->sizeMapY].whichBombIsHere != NULL){
         boom(map->tile[bombToExplode->x][(bombToExplode->y + i) % map->sizeMapY].whichBombIsHere, game, boomMap);
      }
      if (currentItem != NULL && currentItem->ID == INDESTRUCTIBLE_WALL)
         break;
      boomMap[bombToExplode->x][(bombToExplode->y + i) % map->sizeMapY] = 1;
      if (currentItem != NULL && currentItem->ID == WALL)
         break;
      i++;
   }

   i = 1;
   while (i <= bombToExplode->range){
      Item *currentItem = map->tile[(bombToExplode->x - i) % map->sizeMapX][bombToExplode->y].whichItemIsHere;
      if (boomMap[(bombToExplode->x - i) % map->sizeMapX][bombToExplode->y] == 1 && !(currentItem != NULL && (currentItem->ID == WALL || currentItem->ID == INDESTRUCTIBLE_WALL))){
         i++;
         continue;
      }
      if (map->tile[(bombToExplode->x - i) % map->sizeMapX][bombToExplode->y].whichBombIsHere != NULL){
         boom(map->tile[(bombToExplode->x - i) % map->sizeMapX][bombToExplode->y].whichBombIsHere, game, boomMap);
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
      Item *currentItem = map->tile[bombToExplode->x][(bombToExplode->y - i) % map->sizeMapY].whichItemIsHere;
      if (boomMap[bombToExplode->x][(bombToExplode->y - i) % map->sizeMapY] == 1 && !(currentItem != NULL && (currentItem->ID == WALL || currentItem->ID == INDESTRUCTIBLE_WALL))){
         i++;
         continue;
      }
      if (map->tile[bombToExplode->x][(bombToExplode->y - i) % map->sizeMapY].whichBombIsHere != NULL){
         boom(map->tile[bombToExplode->x][(bombToExplode->y - i) % map->sizeMapY].whichBombIsHere, game, boomMap);
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
   BombList *nextBombInList = game->activeBombs;
   if (nextBombInList->thisBomb->x == bombToExplode->x && nextBombInList->thisBomb->y == bombToExplode->y){
      game->activeBombs = nextBombInList->nextOne;
   }
   while (!(nextBombInList->nextOne->thisBomb->x == bombToExplode->x && nextBombInList->nextOne->thisBomb->y == bombToExplode->y)){
      // No need to check if NULL. This bomb can't not be in the list. Y'know, like checking if VIPs in the club are on the list.
      nextBombInList = nextBombInList->nextOne;
   }
   BombList *temp = nextBombInList->nextOne;
   nextBombInList->nextOne = nextBombInList->nextOne->nextOne;
   free(temp);
   free(bombToExplode);
}

void setOffBombs(Game *game){
   if (game->activeBombs == NULL)
      return;
   Map *map = &game->map[game->currentMap];
   BombList *bombToDowntime = game->activeBombs;
   while (bombToDowntime != NULL){
      bombToDowntime->thisBomb->timer--;
      bombToDowntime = bombToDowntime->nextOne;
   }

   short **boomMap = malloc(map->sizeMapX * sizeof(short *));
   for (int i = 0; i < map->sizeMapX; i++){
      boomMap[i] = malloc(map->sizeMapY * sizeof(short));
      for (int j = 0; j < map->sizeMapY; j++){
         boomMap[i][j] = 0;
      }
   }
   printf("debug15\n");
   printf("%p\n", &map->sizeMapX);
   printf("%d\n", map->sizeMapX);
   // The boomMap has two uses : avoid item drop by walls to be destroyed the same turn they poped in and
   // keeping the player from taking multiple damage in one turn. It checks which tile will be cleansed by
   // fire, then it makes things actually go BOOM.

   printf("BOOOOMMAPPP 0\n");
   int sizeX = map->sizeMapX;
   int sizeY = map->sizeMapY;
    printf("BOOOOOOMMAPPP 0.5\n");
    printf("X:%d Y:%d\n", sizeX, sizeY);
    printf("BOOOOMMAPPP 1");
    printf("%p", boomMap);
    printf("BOOOOMMAPPP 2");
   for (int i = 0; i < sizeX; i++){
        printf("debug19");
      for (int j = 0; j < sizeY; j++){
         printf("debug20");
         if (boomMap[i][j] == 1){
            continue;
         }
         printf("debug16");
         Bomb *currentTileBomb = map->tile[i][j].whichBombIsHere;
         if (currentTileBomb != NULL && currentTileBomb->timer <= 0){
             printf("debug17");
            boom(currentTileBomb, game, boomMap);
             printf("debug18");
         }
      }
   }

   // Display of explosions
   clearScreen();
   displayBoomMap(game, boomMap);
   sleep(1);

   // Everything goes BOOM !
   for (int i = 0; i < map->sizeMapX; i++){
      for (int j = 0; j < map->sizeMapY; j++){
         if (boomMap[i][j] == 1){
            if (map->tile[i][j].whoIsHere != NULL){
               hitPlayer(map->tile[i][j].whoIsHere, game);
            }
            if (map->tile[i][j].whichItemIsHere != NULL){
               if (map->tile[i][j].whichItemIsHere->ID == WALL){
                  free(map->tile[i][j].whichItemIsHere);
                  map->tile[i][j].whichItemIsHere = newItem(getRandomItem());
               }
               else{
                  free(map->tile[i][j].whichItemIsHere);
                  map->tile[i][j].whichItemIsHere = NULL;
               }
            }
         }
      }
   }
}
