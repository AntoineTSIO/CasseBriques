//
// Created by Nicolas Pierrot on 26/10/2022.
//
#include "Functions.h"
#include "../Bomb/bomb.h"
#include "../AboutItem/AboutItem.h"
#include "../Socket/Socket.h"

int chooseGameType()
{
    int choice;
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

void deletePlayers(Player *players, int nbPlayersWished)
{
    /*for (int indexNbPlayers = 0; indexNbPlayers < nbPlayersWished; ++indexNbPlayers) {
        free(players[indexNbPlayers]);
    }*/
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
            break;
        case 3:
            char ipAddr;
            printf("Saisir l'adresse IP du serveur à rejoindre' :");
            scanf(" %c", &ipAddr);
            clientStart(ipAddr);
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
        default:
            printf("Saisie incorrecte\n");
            break;
        }
        game.players = *createPlayers(game.numberOfPlayers, game.numberOfHumanPlayers, game.nbBombsPerPlayer);
        game.activeBombs = NULL;
        procedurallyInitMap(&game);
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

    Map procedurallyInitMap(Game * game)
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

        while (map.sizeMapX < 7 || map.sizeMapX > 51)
        {
            printf("Saisir la hauteur de carte souhaitée :");
            scanf(" %hd", &map.sizeMapX);
            getchar();
        }
        while (map.sizeMapY < 7 || map.sizeMapY > 51)
        {
            printf("Saisir la largeur de carte souhaitée :");
            scanf(" %hd", &map.sizeMapY);
            getchar();
        }
        do
        {
            printf("Saisir le nombre de tunnels horizontaux souhaité :");
            scanf(" %hd", &map.numberOfHorizontalTunnels);
            getchar();
        } while (map.numberOfHorizontalTunnels > (int)((map.sizeMapX - 1) / 10));
        do
        {
            printf("Saisir le nombre de tunnels verticaux souhaité :");
            scanf(" %hd", &map.numberOfVerticalTunnels);
            getchar();
        } while (map.numberOfVerticalTunnels > (int)((map.sizeMapY - 1) / 10));
        /*while (map.sizeMapX < 11) {
            printf("Saisir le numéro de carte :\n");
            printf("1 - 11x11\n");
            printf("2 - 11x11 + Zone de téléportation\n");
            printf("3 - 21x21\n");
            printf("4 - 21x21 + Zone de téléportation\n");
            printf("5 - 31x31\n");
            printf("6 - 31x31 + Zone de téléportation\n");
            printf("7 - 41x41\n");
            printf("8 - 41x41 + Zone de téléportation\n");

            char input[256];
            fgets(input, 256, stdin);
            int choice = atoi(input);

            switch (choice) {
                case 1:
                    map.sizeMapX = 11;
                    map.sizeMapY = 11;
                    game.teleportZone = 0;
                    game.maxRange = 5;
                    break;
                case 2:
                    map.sizeMapX = 11;
                    map.sizeMapY = 11;
                    game.teleportZone = 1;
                    game.maxRange = 5;
                    break;
                case 3:
                    map.sizeMapX = 21;
                    map.sizeMapY = 21;
                    game.teleportZone = 0;
                    game.maxRange = 10;
                    break;
                case 4:
                    map.sizeMapX = 21;
                    map.sizeMapY = 21;
                    game.teleportZone = 1;
                    game.maxRange = 10;
                    break;
                case 5:
                    map.sizeMapX = 31;
                    map.sizeMapY = 31;
                    game.teleportZone = 0;
                    game.maxRange = 15;
                    break;
                case 6:
                    map.sizeMapX = 31;
                    map.sizeMapY = 31;
                    game.teleportZone = 1;
                    game.maxRange = 15;
                    break;
                case 7:
                    map.sizeMapX = 41;
                    map.sizeMapY = 41;
                    game.teleportZone = 0;
                    game.maxRange = 20;
                    break;
                case 8:
                    map.sizeMapX = 41;
                    map.sizeMapY = 41;
                    game.teleportZone = 1;
                    game.maxRange = 20;
                    break;
                default:
                    printf("Saisie incorrecte\n");
                    break;
            }
            game.players = *createPlayers(game.numberOfPlayers, game.numberOfHumanPlayers, game.nbBombsPerPlayer);
            game.activeBombs = NULL;
            return game;
        } */

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
        for (int j = 0; j < map.sizeMapY; j++)
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
                free(map.tile[i * (int)(map.sizeMapX / map.numberOfHorizontalTunnels)][0].whichItemIsHere);
                map.tile[i * (int)(map.sizeMapX / map.numberOfHorizontalTunnels)][0].whichItemIsHere = newItem(WALL);
                free(map.tile[i * (int)(map.sizeMapX / map.numberOfHorizontalTunnels - 1)][0].whichItemIsHere);
                map.tile[i * (int)(map.sizeMapX / map.numberOfHorizontalTunnels) - 1][0].whichItemIsHere = newItem(WALL);
                free(map.tile[i * (int)(map.sizeMapX / map.numberOfHorizontalTunnels)][map.sizeMapY - 1].whichItemIsHere);
                map.tile[i * (int)(map.sizeMapX / map.numberOfHorizontalTunnels)][map.sizeMapY - 1].whichItemIsHere = newItem(WALL);
                free(map.tile[i * (int)(map.sizeMapX / map.numberOfHorizontalTunnels - 1)][map.sizeMapY - 1].whichItemIsHere);
                map.tile[i * (int)(map.sizeMapX / map.numberOfHorizontalTunnels) - 1][map.sizeMapY - 1].whichItemIsHere = newItem(WALL);
            }
        }
        if (map.numberOfVerticalTunnels)
        {
            for (int i = 0; i < map.numberOfVerticalTunnels; i++)
            {
                free(map.tile[0][i * (int)(map.sizeMapY / map.numberOfVerticalTunnels)].whichItemIsHere);
                map.tile[0][i * (int)(map.sizeMapY / map.numberOfVerticalTunnels)].whichItemIsHere = newItem(WALL);
                free(map.tile[0][i * (int)(map.sizeMapY / map.numberOfVerticalTunnels) - 1].whichItemIsHere);
                map.tile[0][i * (int)(map.sizeMapY / map.numberOfVerticalTunnels) - 1].whichItemIsHere = newItem(WALL);
                free(map.tile[map.sizeMapX - 1][i * (int)(map.sizeMapY / map.numberOfVerticalTunnels)].whichItemIsHere);
                map.tile[map.sizeMapX - 1][i * (int)(map.sizeMapY / map.numberOfVerticalTunnels)].whichItemIsHere = newItem(WALL);
                free(map.tile[map.sizeMapX - 1][i * (int)(map.sizeMapY / map.numberOfVerticalTunnels) - 1].whichItemIsHere);
                map.tile[map.sizeMapX - 1][i * (int)(map.sizeMapY / map.numberOfVerticalTunnels) - 1].whichItemIsHere = newItem(WALL);
            }
        }
        // Players
        switch (game->numberOfPlayers)
        {
        case 4:
            map.tile[1][map.sizeMapY - 2].whoIsHere = &game->players[3];
            free(map.tile[1][map.sizeMapY - 2].whichItemIsHere);
            map.tile[1][map.sizeMapY - 2].whichItemIsHere = NULL;
            free(map.tile[2][map.sizeMapY - 2].whichItemIsHere);
            map.tile[2][map.sizeMapY - 2].whichItemIsHere = NULL;
            free(map.tile[1][map.sizeMapY - 3].whichItemIsHere);
            map.tile[1][map.sizeMapY - 3].whichItemIsHere = NULL;
            game->players[3].x = 1;
            game->players[3].y = map.sizeMapY - 2;
        case 3:
            map.tile[map.sizeMapX - 2][1].whoIsHere = &game->players[2];
            free(map.tile[map.sizeMapX - 2][1].whichItemIsHere);
            map.tile[map.sizeMapX - 2][1].whichItemIsHere = NULL;
            free(map.tile[map.sizeMapX - 3][1].whichItemIsHere);
            map.tile[map.sizeMapX - 3][1].whichItemIsHere = NULL;
            free(map.tile[map.sizeMapX - 2][2].whichItemIsHere);
            map.tile[map.sizeMapX - 2][2].whichItemIsHere = NULL;
            game->players[2].x = map.sizeMapX - 2;
            game->players[2].y = 1;
        case 2:
            map.tile[map.sizeMapX - 2][map.sizeMapY - 2].whoIsHere = &game->players[1];
            free(map.tile[map.sizeMapX - 2][map.sizeMapY - 2].whichItemIsHere);
            map.tile[map.sizeMapX - 2][map.sizeMapY - 2].whichItemIsHere = NULL;
            free(map.tile[map.sizeMapX - 3][map.sizeMapY - 2].whichItemIsHere);
            map.tile[map.sizeMapX - 3][map.sizeMapY - 2].whichItemIsHere = NULL;
            free(map.tile[map.sizeMapX - 2][map.sizeMapY - 3].whichItemIsHere);
            map.tile[map.sizeMapX - 2][map.sizeMapY - 3].whichItemIsHere = NULL;
            game->players[1].x = map.sizeMapX - 2;
            game->players[1].y = map.sizeMapY - 2;
        case 1:
            map.tile[1][1].whoIsHere = &game->players[0];
            free(map.tile[1][1].whichItemIsHere);
            map.tile[1][1].whichItemIsHere = NULL;
            free(map.tile[2][1].whichItemIsHere);
            map.tile[2][1].whichItemIsHere = NULL;
            free(map.tile[1][2].whichItemIsHere);
            map.tile[1][2].whichItemIsHere = NULL;
            game->players[0].x = 1;
            game->players[0].y = 1;
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

    void displayMap(Game * game)
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

    void deleteTile(Tile * tile)
    {
        free(tile->whichItemIsHere);
        free(tile->whoIsHere);
        free(tile);
    }

    void deleteMap(Game * game)
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

    void deleteGame(Game * game)
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

    Bomb *createBomb(Player * boomer)
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

    void bombKick(Game * game, short direction)
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

    void playerAction(Game * game)
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

    fclose(file);
    game.map = map;
    return game;
}/**/
