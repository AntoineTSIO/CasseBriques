#include <stdio.h>
#include "Structs.h"
#include "Functions.h"

int main() {
    printf("Casse Briques\n");
    int nbJoueursSouhaites = chooseNumberOfPlayers();
    Joueur *joueurs = createPlayers(nbJoueursSouhaites);

    deletePlayers(joueurs, nbJoueursSouhaites);
    return 0;

}