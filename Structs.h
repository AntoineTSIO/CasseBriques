typedef struct
{
    int nbBombesParJoueur;
    int* dimensionsCarte;
    char** carte;
} Cartes;

typedef struct
{
    int id;
    int nbBombes;
    int portee;
    int vie;
    int bouclier;
    int passBomb;
    int invicibilite;
    int nbKills;
} Joueur;

typedef struct
{
    int id;
    int etatBombes;
    int portee;
    int timer;
    Joueur joueur;
} Bombes;