#include <stdio.h>
#include <stdlib.h>
#include "../../Structs.h"
#include "AboutItem.h"

short getRandomItem(){
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

    // srand(time(NULL));    To be put in the function setting the game up. RNG should only be seeded once per game.
    int whichOne = rand() % total_probability;
    if (whichOne < PROBABILITY_OF_NOTHING)
        return NOTHING;
    if (whichOne < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP)
        return BOMB_UP;
    if (whichOne < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN)
        return BOMB_DOWN;
    if (whichOne < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN +
                       PROBABILITY_OF_RANGE_UP)
        return RANGE_UP;
    if (whichOne < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN +
                       PROBABILITY_OF_RANGE_UP + PROBABILITY_OF_RANGE_DOWN)
        return RANGE_DOWN;
    if (whichOne < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN +
                       PROBABILITY_OF_RANGE_UP + PROBABILITY_OF_RANGE_DOWN + PROBABILITY_OF_RANGE_AT_MAX)
        return RANGE_AT_MAX;
    if (whichOne < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN +
                       PROBABILITY_OF_RANGE_UP + PROBABILITY_OF_RANGE_DOWN + PROBABILITY_OF_RANGE_AT_MAX +
                       PROBABILITY_OF_BOMB_PASS)
        return BOMB_PASS;
    if (whichOne < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN +
                       PROBABILITY_OF_RANGE_UP + PROBABILITY_OF_RANGE_DOWN + PROBABILITY_OF_RANGE_AT_MAX +
                       PROBABILITY_OF_BOMB_PASS + PROBABILITY_OF_BOMB_KICK)
        return BOMB_KICK;
    if (whichOne < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN +
                       PROBABILITY_OF_RANGE_UP + PROBABILITY_OF_RANGE_DOWN + PROBABILITY_OF_RANGE_AT_MAX +
                       PROBABILITY_OF_BOMB_PASS + PROBABILITY_OF_BOMB_KICK + PROBABILITY_OF_INVINCIBILITY)
        return INVINCIBILITY;
    if (whichOne < PROBABILITY_OF_NOTHING + PROBABILITY_OF_BOMB_UP + PROBABILITY_OF_BOMB_DOWN +
                       PROBABILITY_OF_RANGE_UP + PROBABILITY_OF_RANGE_DOWN + PROBABILITY_OF_RANGE_AT_MAX +
                       PROBABILITY_OF_BOMB_PASS + PROBABILITY_OF_BOMB_KICK + PROBABILITY_OF_INVINCIBILITY +
                       PROBABILITY_OF_SHIELD)
        return SHIELD;
    return LIFE_UP;
}

Item *newItem(short itemId){
    if (itemId == NOTHING)
        return NULL;
    Item *item = malloc(sizeof(Item));
    item->ID = itemId;
    switch (itemId){
    case INDESTRUCTIBLE_WALL:
        // item->sprite = '█';
        item->sprite = 'x';
        break;
    case WALL:
        // item->sprite = '▒';
        item->sprite = 'm';
        break;
    default:
        item->sprite = '0' + itemId;
        break;
    }
    return item;
}

void pickUpItem(Game *game, short whichOne){
    switch (whichOne){
    case NOTHING:
        return;
    case BOMB_UP:
        game->currentPlayer->totalNumberOfBombs++;
        game->currentPlayer->numberOfBombsLeft++;
        return;
    case BOMB_DOWN:
        if (game->currentPlayer->totalNumberOfBombs > 1)
            game->currentPlayer->totalNumberOfBombs--;
            game->currentPlayer->numberOfBombsLeft--;
        return;
    case RANGE_UP:
        if (game->currentPlayer->range < game->map[game->currentMap].maxRange)
            game->currentPlayer->range++;
        return;
    case RANGE_DOWN:
        if (game->currentPlayer->range > 1)
            game->currentPlayer->range--;
        return;
    case RANGE_AT_MAX:
        game->currentPlayer->range = game->map[game->currentMap].maxRange;
        return;
    case BOMB_PASS:
        game->currentPlayer->interactionWithBombs = 1;
        return;
    case BOMB_KICK:
        game->currentPlayer->interactionWithBombs = 2;
        return;
    case INVINCIBILITY:
        game->currentPlayer->invincibilityTimer = 2;
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
