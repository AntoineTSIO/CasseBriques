#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Structs.h"

int chooseNumberOfPlayers(){
    // Séléction du nombre de joueurs
    int nbJoueursSouhaites = 0;
    do
    {
        printf("Saisir le nombre de joueurs :");
        scanf(" %d", &nbJoueursSouhaites);
        getchar();
    } while (nbJoueursSouhaites <= 0 || nbJoueursSouhaites > 4);

    printf("\nNombre de joueurs : %d", nbJoueursSouhaites);
    return nbJoueursSouhaites;
}

Joueur* createPlayers(int nbJoueursSouhaites){
    // Création des joueurs
    Joueur** joueurs = malloc(nbJoueursSouhaites * sizeof(Joueur));
    int idJoueur =1;
    for (int indexNbJoueursSouhaites = 0; indexNbJoueursSouhaites < nbJoueursSouhaites; ++indexNbJoueursSouhaites)
    {
        Joueur *unJoueur = malloc(sizeof(Joueur));
        unJoueur->id = idJoueur;
        unJoueur->nbBombes = 1;
        unJoueur->portee = 1;
        unJoueur->vie = 3;
        unJoueur->bouclier = 0;
        unJoueur->passBomb = 0;
        unJoueur->invicibilite = 0;
        unJoueur->nbKills = 0;
        joueurs[indexNbJoueursSouhaites] = unJoueur;
    }
    return joueurs;
}

void deletePlayers(Joueur *joueurs, int nbJoueursSouhaites){
    for (int indexNbJoueursSouhaites = 0; indexNbJoueursSouhaites < nbJoueursSouhaites; ++indexNbJoueursSouhaites)
    {
        free(joueurs[indexNbJoueursSouhaites]);
    }
    free(joueurs);
}

int main()
{
    printf("Casse Briques\n");
    int nbJoueursSouhaites = chooseNumberOfPlayers();
    Joueur* joueurs = createPlayers(nbJoueursSouhaites);



    deletePlayers(joueurs, nbJoueursSouhaites);
    return 0;

}