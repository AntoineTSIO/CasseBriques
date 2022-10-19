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
} Joueur;

typedef struct
{
    int id;
    int etatBombes;
    int portee;
    int timer;
    Joueur joueur;
} Bombes;