#include <stdio.h>
#include <stdlib.h>

// Still working on it. Far from finished as of now, but we had to set stuff down about structures, and then I worked on items.

typedef struct{
   Tile** map;
   short max_row, maxcol;
   Player* players;
   int player_turns;
} Game;
typedef struct {
   Player* owner;
   short range;
   char sprite;
   short x;
   short y;
} Bomb;


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

