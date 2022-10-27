
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Items
#define WALL -1
#define INDESTRUCTIBLE_WALL -2
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
//#define DUCK 10000       // Eh, why not transform the player into a duck ?


// Direction
#define DONTMOVE 0
#define XPLUS 1
#define XMINUS 2
#define YPLUS 3
#define YMINUS 4
#define ALLAHU_AKBAR 5


typedef struct {


   short ID; //BOMB_UP, RANGE_AT_MAX, SHIELD, whatever
} Item;

typedef struct {

} Game;

typedef struct {
   char color;
   short x, y;


   short number_of_bombs;
   short range;
   short interaction_with_bombs; // 0 is default, 1 is bomb walking and 2 is bomb kicking
   short invincibility_turns_left;
   short lives;
   bool shield;
} Player;



short get_random_item(){
   int total_probability = PROBABILITY_OF_NOTHING +
                           PROBABILITY_OF_BOMB_UP +
                           PROBABILITY_OF_BOMB_DOWN +
                           PROBABILITY_OF_RANGE_UP +
                           PROBABILITY_OF_RANGE_DOWN +
                           PROBABILITY_OF_RANGE_AT_MAX +
                           PROBABILITY_OF_BOMB_PASS +
                           PROBABILITY_OF_BOMB_KICK +
                           PROBABILITY_OF_INVINCIBILITY +
                           PROBABILITY_OF_SHIELD +
                           PROBABILITY_OF_LIFE_UP;
   
   //srand(time(NULL));    To be put in the function setting the game up. RNG should only be seeded once per game.
   int which_one = rand() % total_probability;
   if (which_one < PROBABILITY_OF_NOTHING)
      return NOTHING;
   if (which_one < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP)
      return BOMB_UP;
   if (which_one < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN)
      return BOMB_DOWN;
   if (which_one < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN +
                   PROBABILITY_OF_RANGE_UP)
      return RANGE_UP;
   if (which_one < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN +
                   PROBABILITY_OF_RANGE_UP + PROBABILITY_OF_RANGE_DOWN)
      return RANGE_DOWN;
   if (which_one < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN +
                   PROBABILITY_OF_RANGE_UP + PROBABILITY_OF_RANGE_DOWN + PROBABILITY_OF_RANGE_AT_MAX)
      return RANGE_AT_MAX;
   if (which_one < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN +
                   PROBABILITY_OF_RANGE_UP + PROBABILITY_OF_RANGE_DOWN + PROBABILITY_OF_RANGE_AT_MAX +
                   PROBABILITY_OF_BOMB_PASS)
      return BOMB_PASS;
   if (which_one < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN +
                   PROBABILITY_OF_RANGE_UP + PROBABILITY_OF_RANGE_DOWN + PROBABILITY_OF_RANGE_AT_MAX +
                   PROBABILITY_OF_BOMB_PASS + PROBABILITY_OF_BOMB_KICK)
      return BOMB_KICK;
   if (which_one < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN +
                   PROBABILITY_OF_RANGE_UP + PROBABILITY_OF_RANGE_DOWN + PROBABILITY_OF_RANGE_AT_MAX +
                   PROBABILITY_OF_BOMB_PASS + PROBABILITY_OF_BOMB_KICK + PROBABILITY_OF_INVINCIBILITY)
      return INVINCIBILITY;
   if (which_one < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN +
                   PROBABILITY_OF_RANGE_UP + PROBABILITY_OF_RANGE_DOWN + PROBABILITY_OF_RANGE_AT_MAX +
                   PROBABILITY_OF_BOMB_PASS + PROBABILITY_OF_BOMB_KICK + PROBABILITY_OF_INVINCIBILITY +
                   PROBABILITY_OF_SHIELD)
      return SHIELD;
   return LIFE_UP;
}

void pick_up_item(Game* game, short which_one){
   switch (which_one)
   {
   case NOTHING:
      return;
   case BOMB_UP:
      game->current_player.number_of_bombs++;
      return;
   case BOMB_DOWN:
   if (game->current_player.number_of_bombs > 1)
         game->current_player.number_of_bombs--;
      return;
   case RANGE_UP:
      if (game->current_player.range < game.max_range)
         game->current_player.range++;
      return;
   case RANGE_DOWN:
      if (game->current_player.range > 1)
         game->current_player.range--;
      return;
   case RANGE_AT_MAX:
      game->current_player.range = game.max_range;
      return;
   case BOMB_PASS:
      game->current_player.interaction_with_bombs = 1;
      return;
   case BOMB_KICK:
      game->current_player.interaction_with_bombs = 2;
      return;
   case INVINCIBILITY:
      game->current_player.invincibility_turns_left = 2;
      return;
   case SHIELD:
      game->current_player.shield = true;
      return;
   case LIFE_UP:
      game->current_player.lives++;
      return;
   default:
      printf("Well, shit happens.");
   }
}

bool player_moves(Game* game, short direction){
   if (direction == DONTMOVE)
      return true;

   Player* mover = Game.currentPlayer;
   short current_x = mover.x, current_y = mover.y, destination_x = mover.x, destination_y = mover.y;

   switch (direction)
   {
   case ALLAHU_AKBAR:
      game->map[destination_x][destination_y]->bomb = create_bomb(game);
      return true;
   case DONTMOVE:
      break;
   case XPLUS:
      destination_x = (destination_x + 1) % game.max_height;
      break;
   case XMINUS:
      destination_x = (destination_x - 1) % game.max_height;
      break;
   case YPLUS:
      destination_y = (destination_y + 1) % game.max_width;
      break;
   case YMINUS:
      destination_y = (destination_y - 1) % game.max_width;
      break;
   default:
      return false;
   }

   Item what_s_there = game->map[destination_x][destination_y]->item;
   Bomb which_bomb_is_there = game->map[destination_x][destination_y]->item;
   switch (what_s_there.ID)
   {
   case INDESTRUCTIBLE_WALL:
   case WALL:
      return true;
   default:
      mover.x = destination_x;
      mover.y = destination_y;
      pick_up_item(game, what_s_there.ID);
      what_s_there.ID = 0;
      break;
   }
   return true;
}

