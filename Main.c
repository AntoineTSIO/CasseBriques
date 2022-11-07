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
    displayMap(game);

    deleteMap(game);


    return 0;
}