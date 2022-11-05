#ifndef structs_h
#define structs_h

typedef struct {
    short ID; //BOMB_UP, RANGE_AT_MAX, SHIELD, whatever
} Item;

typedef struct {
    short id;
    short nbBomb;
    short range;
    short life;
    short shield;
    short passBomb;
    short invincibility;
    short nbKill;
    char color;
    short x, y;
    short interaction_with_bombs; // 0 is default, 1 is bomb walking and 2 is bomb kicking
} Player;

typedef struct {
    short id;
    short stateBomb;
    short range;
    short x;
    short y;
    short timer;
    char sprite;
    Player owner;
} Bomb;

typedef struct {
    char sprite;
    int item;
    int bomb;
} Tile;

typedef struct {
    Tile **tile;
} Map;

typedef struct {
    short sizeMapX;
    short sizeMapY;
    short nbBombsPerPlayer;
    short numberOfPlayers;
    short multiplayer; // Boolean -> 0 = false, 1 = true
    Player *players;
    short playerTurn;
    Player* currentPlayer;
    short max_range;
} Game;

#endif