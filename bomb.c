#include <stdio.h>
#include <stdlib.h>


typedef struct {
    Bomb* thisBomb;
    BombList* nextOne;
} BombList;

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

void setOffBombs(Game* game){
   
   BombList* bombToDowntime = game->activeBombs;
   while (bombToDowntime != NULL){
      bombToDowntime->timer--;
      bombToDowntime = bombToDowntime->nextOne;
   }

   short** boomMap = malloc(game.sizeMapX * sizeof(short *));
   for (int i = 0; i < game.sizeMapX; i++) {
      boomMap[i] = malloc(game.sizeMapY * sizeof(short));
      for (int j = 0; j < game.sizeMapY; j++){
         boomMap[i][j] = 0;
      }
   }

   // The boomMap has two uses : avoid item drop by walls to be destroyed the same turn they poped in and 
   // keeping the player from taking multiple damage in one turn. It checks which will be cleansed by fire,  
   // then it makes things actually go BOOM.
   for (int i = 0; i < game.sizeMapX; i++){
      for (int j = 0; j < game.sizeMapY; j++){
         if (boomMap[i][j] == 1){
            continue;
         }
         Bomb* currentTileBomb = game->map->tile[i][j].whichBombIsHere;
         if (currentTileBomb != NULL && currentTileBomb->timer <= 0){
            boom(currentTileBomb, game, boomMap);
         }
      }
   }

   // Display of explosions goes here
   
   for (int i = 0; i < game.sizeMapX; i++){
      for (int j = 0; j < game.sizeMapY; j++){
         if (boomMap[i][j] == 1){
            if(game->map->tile[i][j].whoIsHere != NULL){
               hitPlayer(game->map->tile[i][j].whoIsHere, game);
            }
            if(game->map->tile[i][j].whichtemIsHere != NULL){
               if(game->map->tile[i][j].whichtemIsHere->ID == WALL){
                  free(game->map->tile[i][j].whichtemIsHere);
                  game->map->tile[i][j].whichtemIsHere = newItem(getRandomItem());;
               }else{
                  free(game->map->tile[i][j].whichtemIsHere);
                  game->map->tile[i][j].whichtemIsHere = NULL;
               }
            }
         }
      }
   }
}

void boom (Bomb* bombToExplode, Game* game, Short** boomMap){
   boomMap[bombToExplode->x][bombToExplode->y] = 1;

   // The 4 next loops are just one done for each direction.
   // The flame of the exmplosion goes up to one wall (and destructs it if able) or up to range, whichever the less.
   // It sets off other bombs it touches and destroys all items in its path.
   int i = 1;
   while (i <= bombToExplode->range){
      short currentItem = game->map->tile[(bombToExplode->x + i) % game->sizeMapX][bombToExplode->y].whichItemIsHere;
      if (boomMap[(bombToExplode->x + i) % game->sizeMapX][bombToExplode->y] == 1
          && !(currentItem != NULL && currentItem->ID == WALL)){
         i++;
         continue;
      }
      if (game->map->tile[(bombToExplode->x + i) % game->sizeMapX][bombToExplode->y].whichBombIsHere != NULL){
         boom (game->map->tile[(bombToExplode->x + i) % game->sizeMapX][bombToExplode->y].whichBombIsHere, game, boomMap);
      }else{
      if (currentItem != NULL && currentItem->ID == INDESTRUCTIBLE_WALL)
         break;
      boomMap[(bombToExplode->x + i) % game->sizeMapX][bombToExplode->y] = 1;
      if (currentItem != NULL && currentItem->ID == WALL)
         break;
      }
      i++;
   }

   i = 1;
   while (i <= bombToExplode->range){
      short currentItem = game->map->tile[bombToExplode->x][(bombToExplode->y + i) % game->sizeMapY].whichItemIsHere;
      if (boomMap[bombToExplode->x][(bombToExplode->y + i) % game->sizeMapY] == 1
          && !(currentItem != NULL && currentItem->ID == WALL)){
         i++;
         continue;
      }
      if (game->map->tile[bombToExplode->x][(bombToExplode->y + i) % game->sizeMapY].whichBombIsHere != NULL){
         boom (game->map->tile[bombToExplode->x][(bombToExplode->y + i) % game->sizeMapY].whichBombIsHere, game, boomMap);
      }
      if (currentItem != NULL && currentItem->ID == INDESTRUCTIBLE_WALL)
         break;
      boomMap[bombToExplode->x][(bombToExplode->y + i) % game->sizeMapY] = 1;
      if (currentItem != NULL && currentItem->ID == WALL)
         break;
      i++;
   }

   i = 1;
   while (i <= bombToExplode->range){
      short currentItem = game->map->tile[(bombToExplode->x - i) % game->sizeMapX][bombToExplode->y].whichItemIsHere;
      if (boomMap[(bombToExplode->x - i) % game->sizeMapX][bombToExplode->y] == 1
          && !(currentItem != NULL && currentItem->ID == WALL)){
         i++;
         continue;
      }
      if (game->map->tile[(bombToExplode->x - i) % game->sizeMapX][bombToExplode->y].whichBombIsHere != NULL){
         boom (game->map->tile[(bombToExplode->x - i) % game->sizeMapX][bombToExplode->y].whichBombIsHere, game, boomMap);
      }
      if (currentItem != NULL && currentItem->ID == INDESTRUCTIBLE_WALL)
         break;
      boomMap[(bombToExplode->x - i) % game->sizeMapX][bombToExplode->y] = 1;
      if (currentItem != NULL && currentItem->ID == WALL)
         break;
      i++;
   }

   i = 1;
   while (i <= bombToExplode->range){
      short currentItem = game->map->tile[bombToExplode->x][(bombToExplode->y - i) % game->sizeMapY].whichItemIsHere;
      if (boomMap[bombToExplode->x][(bombToExplode->y - i) % game->sizeMapY] == 1
          && !(currentItem != NULL && currentItem->ID == WALL)){
         i++;
         continue;
      }
      if (game->map->tile[bombToExplode->x][(bombToExplode->y - i) % game->sizeMapY].whichBombIsHere != NULL){
         boom (game->map->tile[bombToExplode->x][(bombToExplode->y - i) % game->sizeMapY].whichBombIsHere, game, boomMap);
      }
      if (currentItem != NULL && currentItem->ID == INDESTRUCTIBLE_WALL)
         break;
      boomMap[bombToExplode->x + i][(bombToExplode->y - i) % game->sizeMapY] = 1;
      if (currentItem != NULL && currentItem->ID == WALL)
         break;
      i++;
   }

   bombToExplode->owner->numberOfBombsLeft++;

   // Free the bomb that just exploded.
   
   game->map->tile[bombToExplode->x][bombToExplode->y].whichBombIsHere = NULL;
   BombList* nextBombInList = game->activeBombs;
   if (nextBombInList->thisBomb->x == bombToExplode->x && nextBombInList->thisBomb->y == bombToExplode->y){
      game->activeBombs = nextBombInList->nextOne;
   }
   while (!(nextBombInList->nextOne->thisBomb->x == bombToExplode->x && nextBombInList->nextOne->thisBomb->y == bombToExplode->y)){    // No need to check if NULL. At least this bomb is here.
      nextBombInList = nextBombInList->nextOne;
   }
   BombList* temp = nextBombInList->nextOne;
   nextBombInList->nextOne = nextBombInList->nextOne->nextOne;
   free(temp);
   free(bombToExplode);
}

