#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.c"

int main() {
    printf("Casse Briques\n");

    // Séléection du nombre de joueurs
    int nbJoueursSouhaites = 0;
    do{
        printf("Saisir le nombre de joueurs :");
        scanf(" %d",&nbJoueursSouhaites);
        getchar();
    }while(nbJoueursSouhaites<=0 || nbJoueursSouhaites>4);
    
    printf("\nNombre de joueurs : %d", nbJoueursSouhaites);

    // Création des joueurs
    for (int indexNbJoueursSouhaites = 0; indexNbJoueursSouhaites < nbJoueursSouhaites; ++indexNbJoueursSouhaites) {
        
    }
    return 0;
}