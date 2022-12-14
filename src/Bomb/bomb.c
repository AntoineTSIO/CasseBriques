#include "../../Structs.h"
#include "../AboutItem/AboutItem.h"
#include "../Functions/Functions.h"
#include "../Functions/Color.h"

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
               colorRed();
                printf("\u25CE");
               colorReset();
            }
            if (game->map[game->currentMap].tile[i][j].whichItemIsHere != NULL){
                char tempSprite = game->map[game->currentMap].tile[i][j].whichItemIsHere->sprite;
                switch (tempSprite){
                    case 'x':
                        colorYellow();
                        printf("\u25A3");
                        colorReset();
                        break;
                    case 'm':
                        colorGreen();
                        printf("\u25A8");
                        colorReset();
                        break;
                    case 'e':
                    case '_':
                        printf(" ");
                        break;
                    default:
                        printf("%c", tempSprite);
                        break;
                }
            }
            else if (game->map[game->currentMap].tile[i][j].whoIsHere != NULL){
                short playerId;
                for(int k = 0; k != game->numberOfPlayers; k++){
                    if(game->players[k]->x == i && game->players[k]->y == j){
                        playerId = game->players[k]->id;
                    }
                }
                if(game->map[game->currentMap].tile[i][j].whichBombIsHere != NULL){
                    colorHighlight();
                }
                switch (playerId){
                    case 1:
                        colorPurple();
                        break;
                    case 2:
                        colorRed();
                        break;
                    case 3:
                        colorGreen();
                        break;
                    case 4:
                        colorYellow();
                        break;
                }
                printf("\uA66A");
                colorReset();
            }
            else if (game->map[game->currentMap].tile[i][j].whichBombIsHere != NULL){
                printf("\u0E4F");
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
      Item *currentItem = map->tile[((bombToExplode->x + i) + map->sizeMapX) % map->sizeMapX][bombToExplode->y].whichItemIsHere;
      if (boomMap[((bombToExplode->x + i) + map->sizeMapX) % map->sizeMapX][bombToExplode->y] == 1 && !(currentItem != NULL && (currentItem->ID == WALL || currentItem->ID == INDESTRUCTIBLE_WALL))){
         i++;
         continue;
      }
      if (map->tile[((bombToExplode->x + i) + map->sizeMapX) % map->sizeMapX][bombToExplode->y].whichBombIsHere != NULL){
         boom(map->tile[((bombToExplode->x + i) + map->sizeMapX) % map->sizeMapX][bombToExplode->y].whichBombIsHere, game, boomMap);
      }
      else{
         if (currentItem != NULL && currentItem->ID == INDESTRUCTIBLE_WALL)
            break;
         boomMap[((bombToExplode->x + i) + map->sizeMapX) % map->sizeMapX][bombToExplode->y] = 1;
         if (currentItem != NULL && currentItem->ID == WALL)
            break;
      }
      i++;
   }
   i = 1;
   while (i <= bombToExplode->range){
      Item *currentItem = map->tile[bombToExplode->x][((bombToExplode->y + i) + map->sizeMapY) % map->sizeMapY].whichItemIsHere;
      if (boomMap[bombToExplode->x][((bombToExplode->y + i) + map->sizeMapY) % map->sizeMapY] == 1 && !(currentItem != NULL && (currentItem->ID == WALL || currentItem->ID == INDESTRUCTIBLE_WALL))){
         i++;
         continue;
      }
      if (map->tile[bombToExplode->x][((bombToExplode->y + i) + map->sizeMapY) % map->sizeMapY].whichBombIsHere != NULL){
         boom(map->tile[bombToExplode->x][((bombToExplode->y + i) + map->sizeMapY) % map->sizeMapY].whichBombIsHere, game, boomMap);
      }
      if (currentItem != NULL && currentItem->ID == INDESTRUCTIBLE_WALL)
         break;
      boomMap[bombToExplode->x][((bombToExplode->y + i) + map->sizeMapY) % map->sizeMapY] = 1;
      if (currentItem != NULL && currentItem->ID == WALL)
         break;
      i++;
   }
   i = 1;
   while (i <= bombToExplode->range){
      Item *currentItem = map->tile[((bombToExplode->x - i) + map->sizeMapX) % map->sizeMapX][bombToExplode->y].whichItemIsHere;
      if (boomMap[((bombToExplode->x - i) + map->sizeMapX) % map->sizeMapX][bombToExplode->y] == 1 && !(currentItem != NULL && (currentItem->ID == WALL || currentItem->ID == INDESTRUCTIBLE_WALL))){
         i++;
         continue;
      }
      if (map->tile[((bombToExplode->x - i) + map->sizeMapX) % map->sizeMapX][bombToExplode->y].whichBombIsHere != NULL){
         boom(map->tile[((bombToExplode->x - i) + map->sizeMapX) % map->sizeMapX][bombToExplode->y].whichBombIsHere, game, boomMap);
      }
      if (currentItem != NULL && currentItem->ID == INDESTRUCTIBLE_WALL)
         break;
      boomMap[((bombToExplode->x - i) + map->sizeMapX) % map->sizeMapX][bombToExplode->y] = 1;
      if (currentItem != NULL && currentItem->ID == WALL)
         break;
      i++;
   }
   i = 1;
   while (i <= bombToExplode->range){
      Item *currentItem = map->tile[bombToExplode->x][((bombToExplode->y - i) + map->sizeMapY) % map->sizeMapY].whichItemIsHere;
      if (boomMap[bombToExplode->x][((bombToExplode->y - i) + map->sizeMapY) % map->sizeMapY] == 1 && !(currentItem != NULL && (currentItem->ID == WALL || currentItem->ID == INDESTRUCTIBLE_WALL))){
         i++;
         continue;
      }
      if (map->tile[bombToExplode->x][((bombToExplode->y - i) + map->sizeMapY) % map->sizeMapY].whichBombIsHere != NULL){
         boom(map->tile[bombToExplode->x][((bombToExplode->y - i) + map->sizeMapY) % map->sizeMapY].whichBombIsHere, game, boomMap);
      }
      if (currentItem != NULL && currentItem->ID == INDESTRUCTIBLE_WALL)
         break;
      boomMap[bombToExplode->x][((bombToExplode->y - i) + map->sizeMapY) % map->sizeMapY] = 1;
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
      free(nextBombInList);
   } else {
      while (!(nextBombInList->nextOne->thisBomb->x == bombToExplode->x && nextBombInList->nextOne->thisBomb->y == bombToExplode->y)){
         // No need to check if NULL. This bomb can't not be in the list. Y'know, like checking if VIPs in the club are on the list.
         nextBombInList = nextBombInList->nextOne;
      }
      BombList *temp = nextBombInList->nextOne;
      nextBombInList->nextOne = nextBombInList->nextOne->nextOne;
      free(temp);
   }
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
   /*printf("%p\n", &map->sizeMapX);
   printf("%d\n", map->sizeMapX);
   // The boomMap has two uses : avoid item drop by walls to be destroyed the same turn they poped in and
   // keeping the player from taking multiple damage in one turn. It checks which tile will be cleansed by
   // fire, then it makes things actually go BOOM.

   printf("BOOOOMMAPPP 0\n");
   int sizeX = map->sizeMapX;
   int sizeY = map->sizeMapY;
    printf("BOOOOOOMMAPPP 0.5\n");
    printf("X:%d Y:%d\n", sizeX, sizeY);
    //////////  Impossible de savoir pourquoi, mais on a une segfault juste au print suivant. C'est pourtant un print basique, sans rien de particulier.
    //          Du coup, pour le moment, impossible d'avoir des explosions.
    printf("BOOOOMMAPPP 1\n");
    printf("%p\n", boomMap);
    printf("BOOOOMMAPPP 2\n");*/
   for (int i = 0; i < map->sizeMapX; i++){
      
      for (int j = 0; j < map->sizeMapY; j++){
         
         if (boomMap[i][j] == 1){
            continue;
         }
         Bomb *currentTileBomb = map->tile[i][j].whichBombIsHere;
         if (currentTileBomb != NULL && currentTileBomb->timer <= 0){
            
            boom(currentTileBomb, game, boomMap);
         }
      }
   }/**/

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
