#ifndef structs_h
#define structs_h
typedef struct {
    char **carte;
} Cartes;

typedef struct {
    int id;
    int nbBombes;
    int portee;
    int vie;
    int bouclier;
    int passBomb;
    int invicibilite;
    int nbKills;
} Joueur;

typedef struct {
    int id;
    int etatBombes;
    int portee;
    int timer;
    Joueur joueur;
} Bombes;

typedef struct {
    int dimensionsCarteX;
    int dimensionsCarteY;
    int nbBombesParJoueur;
    int nombreJoueurs;
    int multijoueur; // Boolean -> 0 = false, 1 = true
    Joueur **Joueurs;
} Game;

#endif