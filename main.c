#include <stdio.h>
#include "structs.h"
#include "functions.h"

int main() {
    printf("Casse Briques\n");
    Game game = initGame();
    displayStats(game);

    Map map = initMap(game);
    displayMap(map, game);

    int input = keypress();
    printf("%d", input);


    return 0;
}