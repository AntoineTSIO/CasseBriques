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
    printf("db      Cb db   db  .d88b.  .88b  d88. .88b  d88. d88888b       .d8b.  db    db      d8888b.  .d88b.  d8b   db      d8888b. d88888b db    db d8888b. d8888b. d88888b \n");
    printf("88      `D 88   88 .8P  Y8. 88'YbdP`88 88'YbdP`88 88'          d8' `8b 88    88      88  `8D .8P  Y8. 888o  88      88  `8D 88'     88    88 88  `8D 88  `8D 88'     '\n");
    printf("88       ' 88ooo88 88    88 88  88  88 88  88  88 88ooooo      88ooo88 88    88      88oooY' 88    88 88V8o 88      88oooY' 88ooooo 88    88 88oobY' 88oobY' 88ooooo \n");
    printf("88         88~~~88 88    88 88  88  88 88  88  88 88~~~~~      88~~~88 88    88      88~~~b. 88    88 88 V8o88      88~~~b. 88~~~~~ 88    88 88`8b   88`8b   88~~~~~ \n");
    printf("88booo.    88   88 `8b  d8' 88  88  88 88  88  88 88.          88   88 88b  d88      88   8D `8b  d8' 88  V888      88   8D 88.     88b  d88 88 `88. 88 `88. 88.     \n");
    printf("Y88888P    YP   YP  `Y88P'  YP  YP  YP YP  YP  YP Y88888P      YP   YP ~Y8888P'      Y8888P'  `Y88P'  VP   V8P      Y8888P' Y88888P ~Y8888P' 88   YD 88   YD Y88888P \n\n\n");
    Game* game = initGame();
    clearScreen();
    displayGameStats(game);
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