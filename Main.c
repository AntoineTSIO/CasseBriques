#include <stdio.h>
#include "Structs.h"
#include "Functions.h"

int main() {
    clearScreen();
    printf("Casse Briques\n");
    Game game = initGame();
    game.map = initMap(game);
    clearScreen();

    displayStats(game);
    spawnPlayers(game);
    displayMap(game);

    int i = 0;
    while (i < 100) {
        game = playerMovement(game);
        clearScreen();
        displayMap(game);
        i++;
        printf("i = %d\n", i);

    }


    return 0;
}