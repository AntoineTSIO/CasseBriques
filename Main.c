#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Structs.h"
#include "src/Bomb/bomb.h"
#include "src/AboutItem/AboutItem.h"
#include "src/Socket/Socket.h"
#include "src/Functions/Functions.h"

int main(int argc, char **argv)
{
    srand(time(NULL));

    /*SDL_Window *sdlWindow = NULL;

    //SDL version
    SDL_version nb;
    SDL_VERSION(&nb);
    printf("SDL version %d.%d.%d !\n", nb.major, nb.minor, nb.patch);
    //SDL version

    //SDL Init Verification
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //SDL execution...
    sdlWindow = SDL_CreateWindow("Casse Briques en C",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,500,800,SDL_WINDOW_RESIZABLE); // Window title, x pos, y pos, width, height, flags
    if(sdlWindow == NULL)
    {
        SDL_Log("Unable to initialize window: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }*/

    SDL_RenderPresent(sdlRenderer);

    clearScreen();
    printf("Casse Briques\n");
    Game game = initGame();
    clearScreen();

    displayStats(game);
    displayMap(&game);

    int i = 0;
    while (i < 100)
    { // Uniquement pour répéter l'action le temps du dev
        playerAction(&game);
        clearScreen();
        displayMap(&game);
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

    /*SDL_Quit();
    return EXIT_SUCCESS; //*/
    return 0;
}