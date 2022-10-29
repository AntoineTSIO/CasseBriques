#include <stdio.h>
#include "Structs.h"
#include "Functions.h"

int main() {
    printf("Casse Briques\n");
    Game game = initGame();
    displayStats(game);
    // int nbJoueursSouhaites = chooseNumberOfPlayers();
    //Joueur *joueurs = createPlayers(nbJoueursSouhaites);

    deleteGame(game);
    return 0;

}