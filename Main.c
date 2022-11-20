#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Structs.h"
#include "src/Bomb/bomb.h"
#include "src/AboutItem/AboutItem.h"
#include "src/Functions/Functions.h"

int main(int argc, char **argv){
    srand(time(NULL));

    clearScreen();
    printf("L'homme au bon beurre\n");
    Game* game = initGame();
    clearScreen();
    displayGameStats(game);
    //printf("debug2\n");
    for (; game->currentMap < game->numberOfMaps; game->currentMap++){
        displayMapStats(game);


        while (game->numberOfAlivePlayers > 1){
            clearScreen();
            displayMap(game);
            playerAction(game);
            clearScreen();
            displayMap(game);
            sleep(1);
            game->playerTurn++;
            game->currentPlayer = game->players[game->playerTurn % game->numberOfPlayers];
            setOffBombs(game);
        }
        short winner = 0;
        while (game->players[winner] != NULL && !game->players[winner]->life)
                winner++;
        if (winner >= game->numberOfPlayers){
            printf("Je ne crois pas qu'il y ait de bonne ou de mauvaise situation...\n mais là...\n Vous êtes tous morts...\n");
        } else {
            game->players[winner]->numberOfVictories++;
            printf("Bravo joueur %d, tu gagnes cette manche !\n", winner + 1);
        }
        nextMap(game);
    }
    deleteGame(game);

    return 0;
}