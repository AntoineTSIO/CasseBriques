#include "src/lib/sdl2/2.24.2/include/SDL2/SDL.h"
#include <stdio.h>
#include "Structs.h"
#include "Functions.h"

int main(int argc, char **argv) {

    //SDL version
    SDL_version nb;
    SDL_VERSION(&nb);
    printf("SDL version %d.%d.%d !\n", nb.major, nb.minor, nb.patch);
    //SDL version

    clearScreen();
    printf("Casse Briques\n");
    Game game = initGame();
    game.map = initMap(game);
    clearScreen();

    displayStats(game);
    spawnPlayers(game);
    displayMap(game);

    int i = 0;
    while (i < 100) { //Uniquement pour répéter l'action le temps du dev
        game = playerMovement(game);
        clearScreen();
        displayMap(game);
        i++;
        printf("i = %d\n", i);

        // system("/bin/stty raw");
        // int c;
        // system("/bin/stty -echo");
        // c = getc(stdin);
        // system("/bin/stty echo");
        // system("/bin/stty cooked");
        // printf("c = %d\n", c);

    }


    return 0;
}