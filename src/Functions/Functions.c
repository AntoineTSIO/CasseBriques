//
// Created by Nicolas Pierrot on 26/10/2022.
//
#include "Functions.h"
#include "../Bomb/bomb.h"
#include "../AboutItem/AboutItem.h"
#include "../Socket/Socket.h"

int chooseGameType()
{
    int choice = 0;
    while (choice < 1)
    {
        printf("Menu principal\n");
        printf("1 - Démarrer en solo\n");
        printf("2 - Héberger une partie\n");
        printf("3 - Rejoindre une partie\n");
        printf("4 - Démarrer une partie locale (jusqu'à 4 joueurs)\n");

        char input[256];
        fgets(input, 256, stdin);
        choice = atoi(input);
        printf("%d", choice);
        switch (choice)
        {
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
            return 4;
        default:
            printf("Saisie incorrecte\n");
            choice = 0;
            break;
        }
    }
}

Player **createPlayers(int numberOfPlayersWished, int numberOfHumanPlayers, int startingNumberOfBombs)
{
    // Création des players
    Player **players = malloc(numberOfPlayersWished * sizeof(Player *));
    short idPlayers = 1;
    for (int indexNbPlayers = 0; indexNbPlayers < numberOfPlayersWished; ++indexNbPlayers)
    {
        Player *aPlayer = malloc(sizeof(Player));
        aPlayer->id = idPlayers;
        aPlayer->x = 0;
        aPlayer->y = 0;
        aPlayer->nbBomb = startingNumberOfBombs;            // to be modified to take in account the number specified when creating the map
        aPlayer->numberOfBombsLeft = startingNumberOfBombs; // to be modified to take in account the number specified when creating the map
        aPlayer->range = 1;
        aPlayer->life = 3;
        aPlayer->shield = 0;
        aPlayer->invincibilityTimer = 0;
        aPlayer->nbKill = 0;
        aPlayer->interactionWithBombs = 0;
        aPlayer->sprite = 'p';
        aPlayer->numberOfBombsLeft = 1;
        aPlayer->isHuman = (indexNbPlayers < numberOfHumanPlayers);
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
    game.numberOfHumanPlayers = 0;
    game.numberOfAlivePlayers = 0;
    game.multiplayer = 0;
    game.players = NULL;
    game.currentPlayer = NULL;
    game.nbBombsPerPlayer = 0;
    game.playerTurn = 0;
    game.numberOfMaps = 0;
    game.currentMap = 0;
    game.activeBombs = NULL;

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
        game.numberOfPlayers = 4;
        game.numberOfHumanPlayers = 1;
        break;
    case 2:
        serverStart();
        while (game.nbBombsPerPlayer < 1)
        {
            printf("Saisir le nombre de bombes :");
            scanf(" %hd", &game.nbBombsPerPlayer);
            getchar();
        }
        game.numberOfPlayers = 4;
        game.numberOfHumanPlayers = 2;
        game.multiplayer = 1;
        break;
    case 3:
        game.multiplayer = 1;
        char ipAddr;
        printf("Saisir l'adresse IP du serveur à rejoindre' :");
        scanf("%s", &ipAddr);
        clientStart(&ipAddr);
        break;
    case 4:
        game.multiplayer = 1;
        while (game.nbBombsPerPlayer < 1)
        {
            printf("Saisir le nombre de bombes par joueur :");
            scanf(" %hd", &game.nbBombsPerPlayer);
            getchar();
        }
        while (game.numberOfHumanPlayers <= 0 || game.numberOfHumanPlayers > 4)
        {
            printf("Saisir le nombre de joueurs humains :");
            scanf(" %hd", &game.numberOfHumanPlayers);
            getchar();
            if (game.numberOfHumanPlayers <= 1 || game.numberOfHumanPlayers > 4)
                printf("Le nombre de joueurs humains doit être compris entre 1 et 4\n");
        }
        if (game.numberOfHumanPlayers < 4)
        {
            while (game.numberOfPlayers <= game.numberOfHumanPlayers || game.numberOfPlayers > 4)
            {
                printf("Saisir le nombre de joueurs total :");
                scanf(" %hd", &game.numberOfPlayers);
                getchar();
                if (game.numberOfPlayers <= game.numberOfHumanPlayers || game.numberOfPlayers > 4)
                    printf("Le nombre de joueurs total doit être compris entre %d et 4\n", game.numberOfHumanPlayers);
            }
        }
        break;
    }
    game.players = createPlayers(game.numberOfPlayers, game.numberOfHumanPlayers, game.nbBombsPerPlayer);

    while (!game.numberOfMaps || game.numberOfMaps > MAX_NUMBER_OF_MAPS)
    {
        printf("Saisir le nombre de carte jouées :");
        scanf(" %hd", &game.numberOfMaps);
        getchar();
        if (game.numberOfMaps < 1)
            printf("Il faut au moins une carte.\n");
        if (game.numberOfMaps > MAX_NUMBER_OF_MAPS)
            printf("Ça fait trop de cartes, là. On n'est pas dans un TCG !\n");
    }
    int mapTypes[MAX_NUMBER_OF_MAPS];
    for (int i = 0; i < game.numberOfMaps; i++)
    {
        mapTypes[i] = 0;
        printf("Pour chaque carte, choisissez un type de carte.\n");
        while (mapTypes[i] < 1 || mapTypes[i] > 3)
        {
            printf("1. Charger une carte existante\n2. Générer une carte en choisissant ses paramètres\n3. Générer une carte aléatoire.\n");
            scanf(" %d", &mapTypes[i]);
            getchar();
            if (mapTypes[i] < 1 || mapTypes[i] > 3)
                printf("Choix incorrect\n");
        }
    }
    short mapsChecked[MAX_NUMBER_OF_MAPS];
    for (int i = 0; i < MAX_NUMBER_OF_MAPS; i++)
    {
        mapsChecked[i] = NOPE;
    }
    for (int i = 0; i < game.numberOfMaps; i++)
    {
        short checkIndex;
        do
        {
            checkIndex = rand() % game.numberOfMaps;
        } while (mapsChecked[checkIndex] == YUP);
        mapsChecked[checkIndex] = YUP;
        switch (mapTypes[checkIndex])
        {
        case 1:
            // game.map[i] = initMapFromFile(&game);   // to be uncommented when we'll have initMapFromFile()
            break;
        case 2:
            game.map[i] = procedurallyInitMap(&game, USER_DEFINED);
            break;
        case 3:
            game.map[i] = procedurallyInitMap(&game, RANDOMLY_DEFINED);
            break;
        default:
            break;
        }
    }
    return game;
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

Map procedurallyInitMap(Game *game, short isRandomlyDefined)
{
    /*
    x = indestructible wall
    m = destructible wall
    b = bomb
    p = spawn point
    e = empty
    */
    Map map;

    map.sizeMapX = 0;
    map.sizeMapY = 0;
    map.maxRange = 0;
    map.numberOfHorizontalTunnels = 0;
    map.numberOfVerticalTunnels = 0;

    // Get the map dimensions
    if (isRandomlyDefined)
    {
        map.sizeMapX = rand() % 39 + 11;
        map.sizeMapY = rand() % 39 + 11;
        map.numberOfHorizontalTunnels = rand() % ((map.sizeMapX - 1) / 5);
        map.numberOfVerticalTunnels = rand() % ((map.sizeMapY - 1) / 5);
    }
    else
    {
        while (map.sizeMapX < 11 || map.sizeMapX > 51)
        {
            printf("Saisir la hauteur de carte souhaitée (min: 7 | max: 51) :");
            scanf(" %hd", &map.sizeMapX);
            getchar();
            if (map.sizeMapX < 7)
                printf("Carte trop petite.\n");
            if (map.sizeMapX > 51)
                printf("Carte trop grande.\n");
        }
        while (map.sizeMapY < 11 || map.sizeMapY > 51)
        {
            printf("Saisir la largeur de carte souhaitée (min: 7 | max: 51) :");
            scanf(" %hd", &map.sizeMapY);
            getchar();
            if (map.sizeMapY < 7)
                printf("Carte trop petite.\n");
            if (map.sizeMapY > 51)
                printf("Carte trop grande.\n");
        }
        do
        {
            printf("Saisir le nombre de tunnels horizontaux souhaité :");
            scanf(" %hd", &map.numberOfHorizontalTunnels);
            getchar();
            if (map.numberOfHorizontalTunnels > (int)((map.sizeMapX - 1) / 5))
                printf("Eh, pas trop, non plus ! C'est pas un gruyère !\n");
        } while (map.numberOfHorizontalTunnels > (int)((map.sizeMapX - 1) / 5));
        do
        {
            printf("Saisir le nombre de tunnels verticaux souhaité :");
            scanf(" %hd", &map.numberOfVerticalTunnels);
            getchar();
            if (map.numberOfHorizontalTunnels > (int)((map.sizeMapX - 1) / 5))
                printf("Eh, pas trop, non plus ! C'est pas une éponge !\n");
        } while (map.numberOfVerticalTunnels > (int)((map.sizeMapY - 1) / 5));
    }

    // Initialize the map and its tiles
    map.maxRange = map.sizeMapX < map.sizeMapY ? (map.sizeMapX - 1) / 2 : (map.sizeMapY - 1) / 2;
    map.tile = malloc(map.sizeMapX * sizeof(Tile *));
    for (int i = 0; i < map.sizeMapX; i++)
    {
        map.tile[i] = malloc(map.sizeMapY * sizeof(Tile));
    }
    // Center
    for (int i = 1; i < map.sizeMapX - 1; i++)
    {
        for (int j = 1; j < map.sizeMapY - 1; j++)
        {
            map.tile[i][j].whoIsHere = NULL;
            map.tile[i][j].whichBombIsHere = NULL;
            if (i % 2 == 0 && j % 2 == 0 && i != map.sizeMapX - 2 && j != map.sizeMapY - 2)
                map.tile[i][j].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
            else
                map.tile[i][j].whichItemIsHere = newItem(WALL);
        }
    }
    // Borders
    for (int i = 0; i < map.sizeMapX; i++)
    {
        map.tile[i][0].whoIsHere = NULL;
        map.tile[i][0].whichBombIsHere = NULL;
        map.tile[i][0].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
        map.tile[i][map.sizeMapY - 1].whoIsHere = NULL;
        map.tile[i][map.sizeMapY - 1].whichBombIsHere = NULL;
        map.tile[i][map.sizeMapY - 1].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
    }
    for (int j = 1; j < map.sizeMapY - 1; j++)
    {
        map.tile[0][j].whoIsHere = NULL;
        map.tile[0][j].whichBombIsHere = NULL;
        map.tile[0][j].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
        map.tile[map.sizeMapX - 1][j].whoIsHere = NULL;
        map.tile[map.sizeMapX - 1][j].whichBombIsHere = NULL;
        map.tile[map.sizeMapX - 1][j].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
    }
    // Tunnels
    if (map.numberOfHorizontalTunnels)
    {
        for (int i = 0; i < map.numberOfHorizontalTunnels; i++)
        {
            free(map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels)][0].whichItemIsHere);
            map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels)][0].whichItemIsHere = newItem(WALL);
            free(map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels) - 1][0].whichItemIsHere);
            map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels) - 1][0].whichItemIsHere = newItem(WALL);
            if (rand() % 2)
            {
                free(map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels)][map.sizeMapY - 1].whichItemIsHere);
                map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels)][map.sizeMapY - 1].whichItemIsHere = newItem(WALL);
                free(map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels) - 1][map.sizeMapY - 1].whichItemIsHere);
                map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels) - 1][map.sizeMapY - 1].whichItemIsHere = newItem(WALL);
            }
        }
    }
    if (map.numberOfVerticalTunnels)
    {
        for (int i = 0; i < map.numberOfVerticalTunnels; i++)
        {
            free(map.tile[0][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels)].whichItemIsHere);
            map.tile[0][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels)].whichItemIsHere = newItem(WALL);
            free(map.tile[0][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels) - 1].whichItemIsHere);
            map.tile[0][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels) - 1].whichItemIsHere = newItem(WALL);
            if (rand() % 2)
            {
                free(map.tile[map.sizeMapX - 1][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels)].whichItemIsHere);
                map.tile[map.sizeMapX - 1][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels)].whichItemIsHere = newItem(WALL);
                free(map.tile[map.sizeMapX - 1][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels) - 1].whichItemIsHere);
                map.tile[map.sizeMapX - 1][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels) - 1].whichItemIsHere = newItem(WALL);
            }
        }
    }
    // Players
    switch (game->numberOfPlayers)
    {
    case 4:
        map.tile[1][map.sizeMapY - 2].whoIsHere = game->players[3];
        free(map.tile[1][map.sizeMapY - 2].whichItemIsHere);
        map.tile[1][map.sizeMapY - 2].whichItemIsHere = NULL;
        free(map.tile[2][map.sizeMapY - 2].whichItemIsHere);
        map.tile[2][map.sizeMapY - 2].whichItemIsHere = NULL;
        free(map.tile[1][map.sizeMapY - 3].whichItemIsHere);
        map.tile[1][map.sizeMapY - 3].whichItemIsHere = NULL;
        game->players[3]->x = 1;
        game->players[3]->y = map.sizeMapY - 2;
    case 3:
        map.tile[map.sizeMapX - 2][1].whoIsHere = game->players[2];
        free(map.tile[map.sizeMapX - 2][1].whichItemIsHere);
        map.tile[map.sizeMapX - 2][1].whichItemIsHere = NULL;
        free(map.tile[map.sizeMapX - 3][1].whichItemIsHere);
        map.tile[map.sizeMapX - 3][1].whichItemIsHere = NULL;
        free(map.tile[map.sizeMapX - 2][2].whichItemIsHere);
        map.tile[map.sizeMapX - 2][2].whichItemIsHere = NULL;
        game->players[2]->x = map.sizeMapX - 2;
        game->players[2]->y = 1;
    case 2:
        map.tile[map.sizeMapX - 2][map.sizeMapY - 2].whoIsHere = game->players[1];
        free(map.tile[map.sizeMapX - 2][map.sizeMapY - 2].whichItemIsHere);
        map.tile[map.sizeMapX - 2][map.sizeMapY - 2].whichItemIsHere = NULL;
        free(map.tile[map.sizeMapX - 3][map.sizeMapY - 2].whichItemIsHere);
        map.tile[map.sizeMapX - 3][map.sizeMapY - 2].whichItemIsHere = NULL;
        free(map.tile[map.sizeMapX - 2][map.sizeMapY - 3].whichItemIsHere);
        map.tile[map.sizeMapX - 2][map.sizeMapY - 3].whichItemIsHere = NULL;
        game->players[1]->x = map.sizeMapX - 2;
        game->players[1]->y = map.sizeMapY - 2;
    case 1:
        map.tile[1][1].whoIsHere = game->players[0];
        free(map.tile[1][1].whichItemIsHere);
        map.tile[1][1].whichItemIsHere = NULL;
        free(map.tile[2][1].whichItemIsHere);
        map.tile[2][1].whichItemIsHere = NULL;
        free(map.tile[1][2].whichItemIsHere);
        map.tile[1][2].whichItemIsHere = NULL;
        game->players[0]->x = 1;
        game->players[0]->y = 1;
        break;
    default:
        break;
    }

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
    Map *map = &game->map[game->currentMap];
    Player *mover = game->currentPlayer;
    if (!mover->life)
        return;
    short currentX = mover->x, currentY = mover->y, destinationX = mover->x, destinationY = mover->y;
    short direction = MOVEMENT_KEY_ERROR;
    do
    {
        direction = getPlayerAction();
    } while (direction == MOVEMENT_KEY_ERROR || (map->tile[currentX][currentY].whichBombIsHere != NULL && direction == PUT_BOMB));

    switch (direction)
    {
    case PUT_BOMB:
        if (mover->numberOfBombsLeft)
        {
            Bomb *whichBomb = newBomb(game);
            map->tile[destinationX][destinationY].whichBombIsHere = whichBomb; // createBomb : to be defined // WARNING : Can't put a bomb if there's already a bomb there.
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

    Item *whichItemIsHere = map->tile[destinationX][destinationY].whichItemIsHere;
    Bomb *whichBombIsHere = map->tile[destinationX][destinationY].whichBombIsHere;
    if (whichItemIsHere != NULL)
    {
        switch (whichItemIsHere->ID)
        {
        case INDESTRUCTIBLE_WALL:
        case WALL:
            return;
        default:
            mover->x = destinationX;
            mover->y = destinationY;
            map->tile[currentX][currentY].whoIsHere = NULL;
            pickUpItem(game, whichItemIsHere->ID);
            map->tile[destinationX][destinationY].whichItemIsHere = NULL;
            map->tile[destinationX][destinationY].whoIsHere = mover;
            return;
        }
    }
    else if (whichBombIsHere != NULL)
    {
        switch (mover->interactionWithBombs)
        {
        case PIETON:
            return;
        case BOMB_WALK:
            mover->x = destinationX;
            mover->y = destinationY;
            return;
        case BOMB_KICKING:
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
Game initMapFromFile(Game game, char *mapName) {
    Map map;
    FILE *file = fopen(mapName, "r");
    if (file == NULL) {
        printf("Error opening file");
        exit(1);
    }
    fscanf(file, "%hd", &game.map[game.currentMap].sizeMapX);
    fscanf(file, "%hd", &game.map[game.currentMap].sizeMapY);
    map[game->currentMap].tile = malloc(game.map[game.currentMap].sizeMapX * sizeof(Tile *));
    for (int i = 0; i < game.map[game.currentMap].sizeMapX; i++) {
        map[game->currentMap].tile[i] = malloc(game.map[game.currentMap].sizeMapY * sizeof(Tile));
    }
    for (int i = 0; i < game.map[game.currentMap].sizeMapX; i++) {
        for (int j = 0; j < game.map[game.currentMap].sizeMapY; j++) {
            fscanf(file, "%c", &map[game->currentMap].tile[i][j].sprite);
            if (map[game->currentMap].tile[i][j].sprite == 'x') {
                map[game->currentMap].tile[i][j].item = INDESTRUCTIBLE_WALL;
            } else if (map[game->currentMap].tile[i][j].sprite == 'm') {
                map[game->currentMap].tile[i][j].item = WALL;
            } else if (map[game->currentMap].tile[i][j].sprite == 'p') {
                map[game->currentMap].tile[i][j].item = NOTHING;
            } else if (map[game->currentMap].tile[i][j].sprite == 'e') {
                map[game->currentMap].tile[i][j].item = NOTHING;
            }
            map[game->currentMap].tile[i][j].bomb = 0;
        }
    }
    fclose(file);
    game.map = map;
    return game;
}
*/