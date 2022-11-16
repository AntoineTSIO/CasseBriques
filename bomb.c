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
      BombList* nextBomb = game->activeBombs;
      while (nextBomb->nextOne != NULL){ 
         nextBomb = nextBomb->nextOne;
      }
      nextBomb->nextOne = bombNode;
   }
   return bomb;
}

void boom (Bomb* bombToExplode, Game game){
   int i = 1;
   while (i <= bombToExplode.range && (bombToExplode.x + i) < game.map.max_row){
      short current_item = game.map[bombToExplode.x + i][bombToExplode.y].item;
      switch (current_item)
      {
      case INDESTRUCTIBLE_WALL:
         break;
      case DESTRUCTIBLE_WALL:
         current_item = spawn_random_item();
         break;
      default:
         getting_pumped_up(game.players[game.player_turns % 4], current_item);
         current_item = 0;
         break;
      }
   }
   i = 1;
   while (i <= bombToExplode.range && (bombToExplode.y + i) < game.map.max_col){

   }
   i = 1;
   while (i <= bombToExplode.range && (bombToExplode.x - i) >= 0){

   }
   i = 1;
   while (i <= bombToExplode.range && (bombToExplode.x - i) >= 0){

   }
}

