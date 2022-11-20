#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Structs.h"
#include "src/Bomb/bomb.h"
#include "src/AboutItem/AboutItem.h"
#include "src/Socket/Socket.h"
#include "src/Functions/Functions.h"

int main(int argc, char **argv){
    srand(time(NULL));

    clearScreen();
    printf("Casse Briques\n");
    Game game = initGame();
    clearScreen();

    displayStats(game);
    displayMap(&game);

    int i = 0;
    while (i < 100){ // Uniquement pour répéter l'action le temps du dev
        playerAction(&game);
        clearScreen();
        displayMap(&game);
        i++;
        printf("i = %d\n", i);
    }

    return 0;
}