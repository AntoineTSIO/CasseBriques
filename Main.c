#include <stdio.h>
#include "Structs.h"
#include "Functions.h"

int main() {
    clearScreen();
    printf("Casse Briques\n");
    Game game = initGame();
    clearScreen();
    displayStats(game);

    Map map = initMap(game);
    displayMap(map, game);

    deleteMap(map, game);


    return 0;
}