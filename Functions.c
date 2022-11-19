//
// Created by Nicolas Pierrot on 26/10/2022.
//
#include "Functions.h"

Player **createPlayers(int nbPlayersWished)
{
    // Création des players
    Player **players = malloc(nbPlayersWished * sizeof(Player *));
    short idPlayers = 1;
    for (int indexNbPlayers = 0; indexNbPlayers < nbPlayersWished; ++indexNbPlayers)
    {
        Player *aPlayer = malloc(sizeof(Player));
        aPlayer->id = idPlayers;
        aPlayer->nbBomb = 1;            // to be modified to take in account the number specified when creating the map
        aPlayer->numberOfBombsLeft = 1; // to be modified to take in account the number specified when creating the map
        aPlayer->range = 1;
        aPlayer->life = 3;
        aPlayer->shield = 0;
        aPlayer->invincibilityTimer = 0;
        aPlayer->nbKill = 0;
        aPlayer->interactionWithBombs = 0;
        aPlayer->sprite = 'p';
        aPlayer->numberOfBombsLeft = 1;
        players[indexNbPlayers] = aPlayer;
        idPlayers++;
    }
    return players;
}

void deletePlayers(Player **players, int nbPlayersWished)
{
    for (int indexNbPlayers = 0; indexNbPlayers < nbPlayersWished; ++indexNbPlayers)
    {
        free(players[indexNbPlayers]);
    }
    free(players);
}

Game initGame()
{
    Game game;
    printf("Initialisation du jeu\n");
    game.numberOfPlayers = 0;
    game.multiplayer = 0;
    game.players = NULL;
    game.nbBombsPerPlayer = 0;
    game.playerTurn = 0;
    game.currentMap = 0;

    int gameType = chooseGameType();
    switch (gameType)
    {
    case 1:
        while (game.nbBombsPerPlayer < 1)
        {
            printf("Saisir le nombre de bombes :");
            scanf(" %hd", &game.nbBombsPerPlayer);
            getchar();
        }
        game.numberOfPlayers = 1;
    case 2:
        serverStart();
    case 3:
        printf("Saisir l'adresse IP du serveur à rejoindre' :");
        char *ipAddr = scanf(" %hd", &ipAddr);
        clientStart(ipAddr);
    case 4:
        while (game.nbBombsPerPlayer < 1)
        {
            printf("Saisir le nombre de bombes par joueur :");
            scanf(" %hd", &game.nbBombsPerPlayer);
            getchar();
        }
        while (game.numberOfPlayers <= 0 || game.numberOfPlayers > 4)
        {
            printf("Saisir le nombre de joueurs :");
            scanf(" %hd", &game.numberOfPlayers);
            getchar();
            if (game.numberOfPlayers <= 0 || game.numberOfPlayers > 4)
                printf("Le nombre de joueurs doit être compris entre 1 et 4\n");
            if (game.numberOfPlayers > 1)
                game.multiplayer = 1;
        }
    default:
        printf("Saisie incorrecte\n");
        break;
    }
    game.players = *createPlayers(game.numberOfPlayers, game.nbBombsPerPlayer);
    game.activeBombs = NULL;
    return game;
}

void deleteGame(Game *game)
{
    deletePlayers(game->players, game->numberOfPlayers);
    while (game->activeBombs != NULL)
    {
        BombList *temp = game->activeBombs;
        game->activeBombs = game->activeBombs->next;
        free(temp);
    }
    deleteMap(game); // to de defined
}

void displayStats(Game game)
{
    printf("Paramètres de jeu:\n");
    printf("Nombre de joueurs : %d\n", game.numberOfPlayers);
    printf("Nombre de bombes par joueur : %d\n", game.nbBombsPerPlayer);
    if (game.multiplayer == 1)
        printf("Mode: multiplayer\n");
    else
        printf("Mode: solo\n");
    printf("Dimensions de la carte: %d x %d\n", game.map[game.currentMap].sizeMapX, game.map[game.currentMap].sizeMapY);
}

Map procedurallyInitMap(Game *game)
{
    /*
    x = indestructible wall
    m = destructible wall
    b = bomb
    p = spawn point
    e = empty
    */

    game.map.tile = malloc(game.sizeMapX * sizeof(Tile *));
    if (game.map.file == 0)
    {
        for (int i = 0; i < game.sizeMapX; i++)
        {
            game.map.tile[i] = malloc(game.sizeMapY * sizeof(Tile));
        }

        for (int i = 0; i < game.sizeMapX; i++)
        {
            for (int j = 0; j < game.sizeMapY; j++)
            {
                game.map.tile[i][j].sprite = 'e';
                game.map.tile[i][j].item = NOTHING;
                game.map.tile[i][j].bomb = 0;
            }
        }

        for (int i = 0; i < game.sizeMapX; i++)
        {
            game.map.tile[i][0].sprite = 'x';
            game.map.tile[i][game.sizeMapY - 1].sprite = 'x';
            game.map.tile[i][0].item = INDESTRUCTIBLE_WALL;
        }
        for (int j = 0; j < game.sizeMapY; j++)
        {
            game.map.tile[0][j].sprite = 'x';
            game.map.tile[game.sizeMapX - 1][j].sprite = 'x';
            game.map.tile[0][j].item = INDESTRUCTIBLE_WALL;
        }

        for (int i = 1; i < game.sizeMapX - 1; i++)
        {
            for (int j = 1; j < game.sizeMapY - 1; j++)
            {
                if (i % 2 == 0 && j % 2 == 0)
                {
                    game.map.tile[i][j].sprite = 'x';
                    game.map.tile[i][j].item = INDESTRUCTIBLE_WALL;
                }
            }
        }

        switch (game.numberOfPlayers)
        {
        case 1:
            game.map.tile[1][1].sprite = 'p';
            game.map.tile[1][1].item = NOTHING;
            break;
        case 2:
            game.map.tile[1][1].sprite = 'p';
            game.map.tile[1][1].item = NOTHING;
            game.map.tile[game.sizeMapX - 2][game.sizeMapY - 2].sprite = 'p';
            game.map.tile[game.sizeMapX - 2][game.sizeMapY - 2].item = NOTHING;
            break;
        case 3:
            game.map.tile[1][1].sprite = 'p';
            game.map.tile[1][1].item = NOTHING;
            game.map.tile[game.sizeMapX - 2][game.sizeMapY - 2].sprite = 'p';
            game.map.tile[game.sizeMapX - 2][game.sizeMapY - 2].item = NOTHING;
            game.map.tile[game.sizeMapX - 2][1].sprite = 'p';
            game.map.tile[game.sizeMapX - 2][1].item = NOTHING;
            break;
        case 4:
            game.map.tile[1][1].sprite = 'p';
            game.map.tile[1][1].item = NOTHING;
            game.map.tile[game.sizeMapX - 2][game.sizeMapY - 2].sprite = 'p';
            game.map.tile[game.sizeMapX - 2][game.sizeMapY - 2].item = NOTHING;
            game.map.tile[game.sizeMapX - 2][1].sprite = 'p';
            game.map.tile[game.sizeMapX - 2][1].item = NOTHING;
            game.map.tile[1][game.sizeMapY - 2].sprite = 'p';
            game.map.tile[1][game.sizeMapY - 2].item = NOTHING;
        }

        for (int i = 1; i < game.sizeMapX - 1; i++)
        {
            for (int j = 1; j < game.sizeMapY - 1; j++)
            {
                if (game.map.tile[i][j].sprite != 'x' && game.map.tile[i][j].sprite != 'p')
                {
                    game.map.tile[i][j].sprite = 'm';
                    game.map.tile[i][j].item = WALL;
                }
            }
        }

        for (int i = 1; i < game.sizeMapX - 1; i++)
        {
            for (int j = 1; j < game.sizeMapY - 1; j++)
            {
                if (game.map.tile[i][j].sprite == 'm')
                {
                    if (game.map.tile[i - 1][j].sprite == 'p' || game.map.tile[i + 1][j].sprite == 'p' ||
                        game.map.tile[i][j - 1].sprite == 'p' || game.map.tile[i][j + 1].sprite == 'p')
                    {
                        game.map.tile[i][j].sprite = 'e';
                        game.map.tile[i][j].item = NOTHING;
                    }
                }
            }
        }

        if (game.teleportZone == 1)
        {
            game.map.tile[0][game.sizeMapY / 2].sprite = 'e';
            game.map.tile[0][game.sizeMapY / 2].item = NOTHING;
            game.map.tile[game.sizeMapX - 1][game.sizeMapY / 2].sprite = 'e';
            game.map.tile[game.sizeMapX - 1][game.sizeMapY / 2].item = NOTHING;
            game.map.tile[game.sizeMapX / 2][0].sprite = 'e';
            game.map.tile[game.sizeMapX / 2][0].item = NOTHING;
            game.map.tile[game.sizeMapX / 2][game.sizeMapY - 1].sprite = 'e';
            game.map.tile[game.sizeMapX / 2][game.sizeMapY - 1].item = NOTHING;
        }

        return game.map;
    }
    else
    {
        return initMapFromFile(game);
    }
}

Map initMapFromFile(Game game)
{
    Map map;
    FILE *file = fopen(game.map.mapName, "r");
    if (file == NULL)
    {
        printf("Error opening file");
        exit(1);
    }
    fscanf(file, "%hd", &game.sizeMapX);
    fscanf(file, "%hd", &game.sizeMapY);
    map.tile = malloc(game.sizeMapX * sizeof(Tile *));
    for (int i = 0; i < game.sizeMapX; i++)
    {
        map.tile[i] = malloc(game.sizeMapY * sizeof(Tile));
    }

    for (int i = 1; i < game.sizeMapX - 1; i++)
    {
        for (int j = 1; j < game.sizeMapY - 1; j++)
        {
            map.tile[i][j].whoIsHere = NULL;
            map.tile[i][j].whichBombIsHere = NULL;
            if (i % 2 == 0 && j % 2 == 0)
                map.tile[i][j].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
            else
                map.tile[i][j].whichItemIsHere = newItem(WALL);
        }
    }

    for (int i = 0; i < map.sizeMapX; i++)
    {
        map.tile[i][0].whoIsHere = NULL;
        map.tile[i][0].whichBombIsHere = NULL;
        map.tile[i][0].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
        map.tile[i][map.sizeMapY - 1].whoIsHere = NULL;
        map.tile[i][map.sizeMapY - 1].whichBombIsHere = NULL;
        map.tile[i][map.sizeMapY - 1].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
    }
    for (int j = 0; j < map.sizeMapY; j++)
    {
        map.tile[0][j].whoIsHere = NULL;
        map.tile[0][j].whichBombIsHere = NULL;
        map.tile[0][j].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
        map.tile[map.sizeMapX - 1][j].whoIsHere = NULL;
        map.tile[map.sizeMapX - 1][j].whichBombIsHere = NULL;
        map.tile[map.sizeMapX - 1][j].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
    }

    switch (game.numberOfPlayers)
    {
    case 4:
        map.tile[1][game.sizeMapY - 2].whoIsHere = game.players[3];
        free(map.tile[1][game.sizeMapY - 2].whichItemIsHere);
        map.tile[1][game.sizeMapY - 2].whichItemIsHere = NULL;
        free(map.tile[2][game.sizeMapY - 2].whichItemIsHere);
        map.tile[2][game.sizeMapY - 2].whichItemIsHere = NULL;
        free(map.tile[1][game.sizeMapY - 3].whichItemIsHere);
        map.tile[1][game.sizeMapY - 3].whichItemIsHere = NULL;
        game.players[3].x = 1;
        game.players[3].y = game.sizeMapY - 2;
    case 3:
        map.tile[game.sizeMapX - 2][1].whoIsHere = game.players[2];
        free(map.tile[game.sizeMapX - 2][1].whichItemIsHere);
        map.tile[game.sizeMapX - 2][1].whichItemIsHere = NULL;
        free(map.tile[game.sizeMapX - 3][1].whichItemIsHere);
        map.tile[game.sizeMapX - 3][1].whichItemIsHere = NULL;
        free(map.tile[game.sizeMapX - 2][2].whichItemIsHere);
        map.tile[game.sizeMapX - 2][2].whichItemIsHere = NULL;
        game.players[2].x = game.sizeMapX - 2;
        game.players[2].y = 1;
    case 2:
        map.tile[game.sizeMapX - 2][game.sizeMapY - 2].whoIsHere = game.players[1];
        free(map.tile[game.sizeMapX - 2][game.sizeMapY - 2].whichItemIsHere);
        map.tile[game.sizeMapX - 2][game.sizeMapY - 2].whichItemIsHere = NULL;
        free(map.tile[game.sizeMapX - 3][game.sizeMapY - 2].whichItemIsHere);
        map.tile[game.sizeMapX - 3][game.sizeMapY - 2].whichItemIsHere = NULL;
        free(map.tile[game.sizeMapX - 2][game.sizeMapY - 3].whichItemIsHere);
        map.tile[game.sizeMapX - 2][game.sizeMapY - 3].whichItemIsHere = NULL;
        game.players[1].x = game.sizeMapX - 2;
        game.players[1].y = game.sizeMapY - 2;
    case 1:
        map.tile[1][1].whoIsHere = game.players[0];
        free(map.tile[1][1].whichItemIsHere);
        map.tile[1][1].whichItemIsHere = NULL;
        free(map.tile[2][1].whichItemIsHere);
        map.tile[2][1].whichItemIsHere = NULL;
        free(map.tile[1][2].whichItemIsHere);
        map.tile[1][2].whichItemIsHere = NULL;
        game.players[0].x = 1;
        game.players[0].y = 1;
        break;
    default:
        break;
    }

    /*      to be reworked : maybe replace teleportZone with verticalTunnel and horizontalTunnel, or
             even randomly spawn tunnels depending on map size
        if (game.teleportZone == 1) {
            map.tile[0][game.sizeMapY / 2].sprite = 'e';
            map.tile[0][game.sizeMapY / 2].item = NOTHING;
            map.tile[game.sizeMapX - 1][game.sizeMapY / 2].sprite = 'e';
            map.tile[game.sizeMapX - 1][game.sizeMapY / 2].item = NOTHING;
            map.tile[game.sizeMapX / 2][0].sprite = 'e';
            map.tile[game.sizeMapX / 2][0].item = NOTHING;
            map.tile[game.sizeMapX / 2][game.sizeMapY - 1].sprite = 'e';
            map.tile[game.sizeMapX / 2][game.sizeMapY - 1].item = NOTHING;

        }
    /**/
    return map;
}

void displayMap(Game *game)
{
    // x = █
    // m = ▒
    // p =
    for (int i = 0; i < game->map[game->currentMap].sizeMapX; ++i)
    {
        for (int j = 0; j < game->map[game->currentMap].sizeMapY; ++j)
        {
            if (game->map[game->currentMap].tile[i][j].whichItemIsHere != NULL)
            {
                printf("%c", game->map[game->currentMap].tile[i][j].whichItemIsHere->sprite);
            }
            else if (game->map[game->currentMap].tile[i][j].whoIsHere != NULL)
            { // Penser à afficher bombe ET joueurs présents sur la même case. Impossible avec juste des char, mais possible avec SDL.
                printf("%c", game->map[game->currentMap].tile[i][j].whoIsHere->sprite);
            }
            else if (game->map[game->currentMap].tile[i][j].whichBombIsHere != NULL)
            {
                printf("%c", game->map[game->currentMap].tile[i][j].whichBombIsHere->sprite);
            }
            else
                printf(" ");
            /*if (game->map[game->currentMap].tile[i][j].sprite == 'x') {
                printf("█");
            } else if (game->map[game->currentMap].tile[i][j].sprite == 'm') {
                printf("▒");
            } else if (game->map[game->currentMap].tile[i][j].sprite == 'p') {
                printf("p");
            } else if (game->map[game->currentMap].tile[i][j].sprite == 'b') {
                printf("b");
            } else if (game->map[game->currentMap].tile[i][j].sprite == 'e' || game->map[game->currentMap].tile[i][j].sprite == '_') {
                printf(" ");
            }*/
        }
        printf("\n");
    }
}

void deleteTile(Tile *tile)
{
    free(tile->whichItemIsHere);
    free(tile->whoIsHere);
    free(tile);
}

void deleteMap(Game *game)
{
    for (int i = 0; i < game->map[game->currentMap].sizeMapX; i++)
    {
        for (int j = 0; j < game->map[game->currentMap].sizeMapY; j++)
        {
            deleteTile(&game->map[game->currentMap].tile[i][j]);
        }

        free(game->map[game->currentMap].tile[i]);
    }
    free(game->map[game->currentMap].tile);
}

void deleteGame(Game *game)
{
    deletePlayers(game->players, game->numberOfPlayers);
    while (game->activeBombs != NULL)
    {
        BombList *temp = game->activeBombs;
        game->activeBombs = game->activeBombs->nextOne;
        free(temp);
    }
    deleteMap(game); // to de defined
}

// detect key pressed
char keypress()
{
    system("/bin/stty raw");
    int c;
    system("/bin/stty -echo");
    c = getc(stdin);
    system("/bin/stty echo");
    system("/bin/stty cooked");

    switch (c)
    {
    case 122:
        return 'z';
    case 90:
        return 'z'; // Z
    case 113:
        return 'q';
    case 81:
        return 'q'; // Q
    case 115:
        return 's';
    case 83:
        return 's'; // S
    case 100:
        return 'd';
    case 68:
        return 'd'; // D
    case 101:
        return 'e';
    case 69:
        return 'e'; // E
    case 97:
        return 'a';
    case 65:
        return 'a'; // A
    default:
        return ' ';
    }
}

void clearScreen()
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

Bomb *createBomb(Player *boomer)
{
    Bomb *bomb = malloc(sizeof(Bomb));
    bomb->range = boomer->range;
    bomb->x = boomer->x;
    bomb->y = boomer->y;
    bomb->timer = 4;
    bomb->sprite = 'b';
    bomb->owner = boomer;
    return bomb;
}

void bombKick(Game *game, short direction)
{
    Bomb *kickMe;
    Tile *originTile, *destinationTile, *nextTile;
    int howFarItGoes = 1;

    originTile = &game->map[game->currentMap].tile[game->currentPlayer->x][game->currentPlayer->y];
    nextTile = originTile;
    kickMe = originTile->whichBombIsHere;

    switch (direction)
    {
    case XMINUS:
        do
        {
            destinationTile = nextTile;
            howFarItGoes++;
            nextTile = &game->map[game->currentMap].tile[(game->currentPlayer->x - howFarItGoes) % game->map[game->currentMap].sizeMapX][game->currentPlayer->y];
        } while (nextTile->whichBombIsHere == NULL && nextTile->whichItemIsHere == NULL && nextTile->whoIsHere == NULL);
        break;
    case XPLUS:
        do
        {
            destinationTile = nextTile;
            howFarItGoes++;
            nextTile = &game->map[game->currentMap].tile[(game->currentPlayer->x + howFarItGoes) % game->map[game->currentMap].sizeMapX][game->currentPlayer->y];
        } while (nextTile->whichBombIsHere == NULL && nextTile->whichItemIsHere == NULL && nextTile->whoIsHere == NULL);
        break;
    case YMINUS:
        do
        {
            destinationTile = nextTile;
            howFarItGoes++;
            nextTile = &game->map[game->currentMap].tile[game->currentPlayer->x][(game->currentPlayer->y - howFarItGoes) % game->map[game->currentMap].sizeMapY];
        } while (nextTile->whichBombIsHere == NULL && nextTile->whichItemIsHere == NULL && nextTile->whoIsHere == NULL);
        break;
    case YPLUS:
        do
        {
            destinationTile = nextTile;
            howFarItGoes++;
            nextTile = &game->map[game->currentMap].tile[game->currentPlayer->x][(game->currentPlayer->y + howFarItGoes) % game->map[game->currentMap].sizeMapY];
        } while (nextTile->whichBombIsHere == NULL && nextTile->whichItemIsHere == NULL && nextTile->whoIsHere == NULL);
        break;
    default:
        break;
    }
    destinationTile->whichBombIsHere = kickMe;
    originTile->whichBombIsHere = NULL;
}

short getPlayerAction()
{
    char key = keypress();
    switch (key)
    {
    case 'z':
        return XMINUS;
    case 'q':
        return YMINUS;
    case 's':
        return XPLUS;
    case 'd':
        return YPLUS;
    case 'e':
        return DONT_MOVE;
    case 'a':
        return PUT_BOMB;
    }
    return MOVEMENT_KEY_ERROR;
}

void playerAction(Game *game)
{
    Player *mover = game->currentPlayer;
    if (!mover->life)
        return;
    short currentX = mover->x, currentY = mover->y, destinationX = mover->x, destinationY = mover->y;
    short direction = MOVEMENT_KEY_ERROR;
    do
    {
        direction = getPlayerAction();
    } while (direction == MOVEMENT_KEY_ERROR || (map->tile[currentX][currentY].whichBombIsHere != NULL && direction == PUT_BOMB));

    if (direction == DONT_MOVE)
        return;

    switch (direction)
    {
    case PUT_BOMB:
        if (mover->numberOfBombsLeft)
        {
            map->tile[destinationX][destinationY].whichBombIsHere = createBomb(game); // createBomb : to be defined // WARNING : Can't put a bomb if there's already a bomb there.
            mover->numberOfBombsLeft--;
        }
        return;
    case DONT_MOVE:
        return;
    case XPLUS:
        destinationX = (destinationX + 1) % map->sizeMapX;
        break;
    case XMINUS:
        destinationX = (destinationX - 1) % map->sizeMapX;
        break;
    case YPLUS:
        destinationY = (destinationY + 1) % map->sizeMapY;
        break;
    case YMINUS:
        destinationY = (destinationY - 1) % map->sizeMapY;
        break;
    default:
        return;
    }

    Item whichItemIsHere = game->map->tile[destinationX][destinationY]->whichItemIsHere;
    Bomb whichBombIsHere = game->map->tile[destinationX][destinationY]->whichBombIsHere;
    if (whichItemIsHere != NULL)
    {
        switch (whichItemIsHere.ID)
        {
        case INDESTRUCTIBLE_WALL:
        case WALL:
            return;
        default:
            mover->x = destinationX;
            mover->y = destinationY;
            game->map->tile[currentX][currentY].whoIsHere = NULL;
            pickUpItem(game, whichItemIsHere);
            game->map->tile[destinationX][destinationY].whichItemIsHere = NULL;
            game->map->tile[destinationX][destinationY].whoIsHere = mover;
            return;
        }
    }
    if (whichBombIsHere != NULL)
    {
        switch (mover.interactionWithBombs)
        {
        case PIETON:
            return;
        case BOMB_WALK:
            mover->x = destinationX;
            mover->y = destinationY;
            return;
        case BOMB_KICK:
            bombKick(game, direction);
            return;
        }
    }
}

void hitPlayer(Player *dommageCollateral, Game *game)
{
    if (dommageCollateral->invincibilityTimer)
        return;
    if (dommageCollateral->shield)
    {
        dommageCollateral->shield = 0;
        return;
    }
    dommageCollateral->life--;
    if (!dommageCollateral->life)
    {
        game->map[game->currentMap].tile[dommageCollateral->x][dommageCollateral->y].whoIsHere = NULL;
        // Maybe add something so the game knows that player is dead ? Not really needed, but, eh, why not ?
    }
}

/*
Game spawnPlayers(Game game){
    for(int i = 0; i < game.numberOfPlayers; i++){
        for(int j = 0; j < game.map[game.currentMap].sizeMapX; j++){
            for(int k = 0; k < game.map[game.currentMap].sizeMapY; k++){
                if(game.map[game->currentMap].tile[j][k].sprite == 'p'){
                    game.players[i].x = j;
                    game.players[i].y = k;
                    game.map[game->currentMap].tile[j][k].sprite = 'p';
                    break;
                }
            }
        }
    }
    return game;
}
*/

Game initMapFromFile(Game game, char *mapName)
{
    Map map;
    FILE *file = fopen(mapName, "r");
    if (file == NULL)
    {
        printf("Error opening file");
        exit(1);
    }
    fscanf(file, "%hd", &game.map[game.currentMap].sizeMapX);
    fscanf(file, "%hd", &game.map[game.currentMap].sizeMapY);
    map[game->currentMap].tile = malloc(game.map[game.currentMap].sizeMapX * sizeof(Tile *));
    for (int i = 0; i < game.map[game.currentMap].sizeMapX; i++)
    {
        map[game->currentMap].tile[i] = malloc(game.map[game.currentMap].sizeMapY * sizeof(Tile));
    }
    for (int i = 0; i < game.map[game.currentMap].sizeMapX; i++)
    {
        for (int j = 0; j < game.map[game.currentMap].sizeMapY; j++)
        {
            fscanf(file, "%c", &map[game->currentMap].tile[i][j].sprite);
            if (map[game->currentMap].tile[i][j].sprite == 'x')
            {
                map[game->currentMap].tile[i][j].item = INDESTRUCTIBLE_WALL;
            }
            else if (map[game->currentMap].tile[i][j].sprite == 'm')
            {
                map[game->currentMap].tile[i][j].item = WALL;
            }
            else if (map[game->currentMap].tile[i][j].sprite == 'p')
            {
                map[game->currentMap].tile[i][j].item = NOTHING;
            }
            else if (map[game->currentMap].tile[i][j].sprite == 'e')
            {
                map[game->currentMap].tile[i][j].item = NOTHING;
            }
            map[game->currentMap].tile[i][j].bomb = 0;
        }
    }

    fclose(file);
    game.map = map;
    return game;
}
