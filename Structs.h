#ifndef structs_h
#define structs_h

typedef struct
{
    short id;
    short nbBomb;
    short numberOfBombsLeft;
    short range;
    short life;
    short shield;
    short invincibilityTimer;
    short nbKill;
    char color;
    short x, y;
    short interactionWithBombs; // 0 is default, 1 is bomb walking and 2 is bomb kicking
    char sprite;
    short isHuman;
} Player;

typedef struct
{
    // short id;
    // short stateBomb; // What's that ?
    short range;
    short x, y;
    short timer;
    char sprite;
    Player *owner;
} Bomb;

typedef struct
{
    short ID; // BOMB_UP, RANGE_AT_MAX, SHIELD, whatever
    char sprite;
} Item;

typedef struct
{
    Player *whoIsHere;
    Bomb *whichBombIsHere;
    Item *whichItemIsHere;
} Tile;

typedef struct
{
    short sizeMapX;
    short sizeMapY;
    short maxRange;
    short numberOfVerticalTunnels;
    short numberOfHorizontalTunnels;
    Tile **tile;
    short file;
    char *mapName;
} Map;

typedef struct BombList
{
    Bomb *thisBomb;
    struct BombList *nextOne;
} BombList;

typedef struct
{
    short nbBombsPerPlayer;
    short numberOfPlayers;
    short numberOfAlivePlayers;
    short numberOfHumanPlayers;
    short multiplayer; // Boolean -> 0 = false, 1 = true
    Player *players;
    short playerTurn;
    Player *currentPlayer;
    short currentMap;
    Map *map;
    BombList *activeBombs;
} Game;

#endif