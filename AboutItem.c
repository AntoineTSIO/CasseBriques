#include <stdio.h>
#include <stdlib.h>
#include "Structs.h"
#include "AboutItem.h"

short get_random_item() {
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

void pick_up_item(Game *game, short which_one) {
    switch (which_one) {
        case NOTHING:
            return;
        case BOMB_UP:
            game->currentPlayer->nbBomb++;
            return;
        case BOMB_DOWN:
            if (game->currentPlayer->nbBomb > 1)
                game->currentPlayer->nbBomb--;
            return;
        case RANGE_UP:
            if (game->currentPlayer->range < game->max_range)
                game->currentPlayer->range++;
            return;
        case RANGE_DOWN:
            if (game->currentPlayer->range > 1)
                game->currentPlayer->range--;
            return;
        case RANGE_AT_MAX:
            game->currentPlayer->range = game->max_range;
            return;
        case BOMB_PASS:
            game->currentPlayer->interaction_with_bombs = 1;
            return;
        case BOMB_KICK:
            game->currentPlayer->interaction_with_bombs = 2;
            return;
        case INVINCIBILITY:
            game->currentPlayer->invincibility = 2;
            return;
        case SHIELD:
            game->currentPlayer->shield = 1;
            return;
        case LIFE_UP:
            game->currentPlayer->life++;
            return;
        default:
            printf("Well, shit happens.");
    }
}

/*int player_moves(Game *game, short direction, Map *map) {
    if (direction == DONTMOVE)
        return 1;

    Player *mover = game->currentPlayer;
    short current_x = mover->x, current_y = mover->y, destination_x = mover->x, destination_y = mover->y;

    switch (direction) {
        case EXPLOSION:
            map->tile[destination_x][destination_y].bomb = create_bomb(game);
            return 1;
        case DONTMOVE:
            break;
        case XPLUS:
            destination_x = (destination_x + 1) % game->sizeMapX;
            break;
        case XMINUS:
            destination_x = (destination_x - 1) % game->sizeMapX;
            break;
        case YPLUS:
            destination_y = (destination_y + 1) % game->sizeMapY;
            break;
        case YMINUS:
            destination_y = (destination_y - 1) % game->sizeMapY;
            break;
        default:
            return 0;
    }

    Item what_s_there = map->tile[destination_x][destination_y].item;
    Bomb which_bomb_is_there = map->tile[destination_x][destination_y].item;
    switch (what_s_there.ID) {
        case INDESTRUCTIBLE_WALL:
        case WALL:
            return 1;
        default:
            mover->x = destination_x;
            mover->y = destination_y;
            pick_up_item(game, what_s_there.ID);
            what_s_there.ID = 0;
            break;
    }
    return 1;
}*/
