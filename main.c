#include <stdio.h>
#include "structs.h"
#include "functions.h"

int main() {
    printf("Casse Briques\n");
    Game game = initGame();
    displayStats(game);
    // int nbJoueursSouhaites = chooseNumberOfPlayers();
    //Joueur *joueurs = createPlayers(nbJoueursSouhaites);

    deleteGame(game);
    return 0;

}