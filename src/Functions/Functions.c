#include "Functions.h"
#include "../Bomb/bomb.h"
#include "../AboutItem/AboutItem.h"
#include "Color.h"

int chooseGameType(){
    int choice = 0;
    while (choice < 1){
        printf("Menu principal\n");
        printf("1 - Démarrer en solo\n");
        printf("2 - Héberger une partie\n");
        printf("3 - Rejoindre une partie\n");
        printf("4 - Démarrer une partie locale (jusqu'à 4 joueurs)\n");

        char input[256];
        fgets(input, 256, stdin);
        choice = atoi(input);
        switch (choice){
        case 1:
            return 1;
        case 2:
            printf("socket non fonctionnel, veuillez choisir un autre mode de jeu");
            choice = 0;
            break;
        case 3:
            printf("socket non fonctionnel, veuillez choisir un autre mode de jeu");
            choice = 0;
            break;
        case 4:
            return 4;
        default:
            printf("Saisie incorrecte\n");
            choice = 0;
            break;
        }
    }
}

Player **createPlayers(int numberOfPlayersWished, int numberOfHumanPlayers){
    // Création des players
    Player **players = malloc(numberOfPlayersWished * sizeof(Player *));
    short idPlayers = 1;
    for (int indexNbPlayers = 0; indexNbPlayers < numberOfPlayersWished; ++indexNbPlayers){
        Player *aPlayer = malloc(sizeof(Player));
        aPlayer->id = idPlayers;
        aPlayer->x = 0;
        aPlayer->y = 0;
        aPlayer->totalNumberOfBombs = 1;            // to be modified to take in account the number specified when creating the map
        aPlayer->numberOfBombsLeft = 1; // to be modified to take in account the number specified when creating the map
        aPlayer->range = 1;
        aPlayer->life = 2;
        aPlayer->shield = 0;
        aPlayer->invincibilityTimer = 0;
        aPlayer->numberOfVictories = 0;
        aPlayer->interactionWithBombs = 0;
        aPlayer->sprite = 'p';
        aPlayer->numberOfBombsLeft = 1;
        aPlayer->isHuman = (indexNbPlayers < numberOfHumanPlayers);
        players[indexNbPlayers] = aPlayer;
        idPlayers++;
    }
    return players;
}

void deletePlayers(Player **players, int nbPlayersWished){
    /*for (int indexNbPlayers = 0; indexNbPlayers < nbPlayersWished; ++indexNbPlayers){
        free(players[indexNbPlayers]);
    }*/
    free(players);
}

Game* initGame(){
    Game* game = malloc(sizeof(Game));
    printf("Initialisation du jeu\n");
    game->numberOfPlayers = 0;
    game->numberOfHumanPlayers = 0;
    game->numberOfAlivePlayers = 0;
    game->multiplayer = 0;
    game->players = NULL;
    game->currentPlayer = NULL;
    game->playerTurn = 0;
    game->numberOfMaps = 0;
    game->currentMap = 0;
    game->activeBombs = NULL;

    int gameType = chooseGameType();
    switch (gameType){
        case 1:
            game->numberOfPlayers = 0;
            game->numberOfHumanPlayers = 1;
            while (game->numberOfPlayers < 2 || game->numberOfPlayers > 4){
                printf("Saisir le nombre de joueurs total :");
                scanf(" %hd", &game->numberOfPlayers);
                getchar();
                if (game->numberOfPlayers < 2 || game->numberOfPlayers > 4)
                    printf("Le nombre de joueurs total doit être compris entre 2 et 4\n");
            }
            break;
        case 2:
            /*
             * game->numberOfPlayers = 4;
             * game->numberOfHumanPlayers = 2;
             * game->multiplayer = 1;
             * break;
             */
            printf("socket non fonctionnel, veuillez choisir un autre mode de jeu");
            break;
        case 3:
            /*
             * game->multiplayer = 1;
             * char ipAddr;
             * printf("Saisir l'adresse IP du serveur à rejoindre' :");
             * scanf("%s", &ipAddr);
             * clientStart(&ipAddr);
             */
            printf("socket non fonctionnel, veuillez choisir un autre mode de jeu");
            break;
        case 4:
            game->multiplayer = 1;
            while (game->numberOfHumanPlayers < 1 || game->numberOfHumanPlayers > 4){
                printf("Saisir le nombre de joueurs humains :");
                scanf(" %hd", &game->numberOfHumanPlayers);
                getchar();
                if (game->numberOfHumanPlayers < 1 || game->numberOfHumanPlayers > 4)
                    printf("Le nombre de joueurs humains doit être compris entre 1 et 4\n");
            }
            if (game->numberOfHumanPlayers < 4){
                while (game->numberOfPlayers < game->numberOfHumanPlayers || game->numberOfPlayers < 2 || game->numberOfPlayers > 4){
                    printf("Saisir le nombre de joueurs total :");
                    scanf(" %hd", &game->numberOfPlayers);
                    getchar();
                    if (game->numberOfPlayers < game->numberOfHumanPlayers || game->numberOfPlayers < 2 || game->numberOfPlayers > 4)
                        printf("Le nombre de joueurs total doit être compris entre %d et 4\n", (game->numberOfHumanPlayers < 2 ? 2 : game->numberOfHumanPlayers));
                }
            } else {
                game->numberOfPlayers = 4;
            }
            break;
    }
    game->players = createPlayers(game->numberOfPlayers, game->numberOfHumanPlayers);
    game->currentPlayer = game->players[0];
    game->numberOfAlivePlayers = game->numberOfPlayers;

    while (!game->numberOfMaps || game->numberOfMaps > MAX_NUMBER_OF_MAPS){
        printf("Saisir le nombre de carte jouées :");
        scanf(" %hd", &game->numberOfMaps);
        getchar();
        if (game->numberOfMaps < 1)
            printf("Il faut au moins une carte.\n");
        if (game->numberOfMaps > MAX_NUMBER_OF_MAPS)
            printf("Ça fait trop de cartes, là. On n'est pas dans un TCG !\n");
    }
    game->map = malloc(game->numberOfMaps * sizeof(Map));
    int mapTypes[MAX_NUMBER_OF_MAPS];
    for (int i = 0; i < game->numberOfMaps; i++){
        mapTypes[i] = 0;
        printf("Pour chaque carte, choisissez un type de carte.\n");
        while (mapTypes[i] < 1 || mapTypes[i] > 3){
            printf("1. Charger une carte existante\n2. Générer une carte en choisissant ses paramètres\n3. Générer une carte aléatoire.\n");
            scanf(" %d", &mapTypes[i]);
            getchar();
            if (mapTypes[i] < 1 || mapTypes[i] > 3)
                printf("Choix incorrect\n");
        }
    }

    short mapsChecked[MAX_NUMBER_OF_MAPS];
    for (int i = 0; i < MAX_NUMBER_OF_MAPS; i++){
        mapsChecked[i] = NOPE;
    }
    for (int i = 0; i < game->numberOfMaps; i++){
        short checkIndex;
        do{
            checkIndex = rand() % game->numberOfMaps;
        } while (mapsChecked[checkIndex] == YUP);
        mapsChecked[checkIndex] = YUP;
        char* filename = malloc(sizeof(char) * 100);
        switch (mapTypes[checkIndex]){
        case 1:
            filename = "maskedMap.map";
            game->map[i] = initMapFromFile(game, filename);   // to be uncommented when we'll have initMapFromFile()
            break;
        case 2:
            game->map[i] = procedurallyInitMap(game, USER_DEFINED);
            break;
        case 3:
            game->map[i] = procedurallyInitMap(game, RANDOMLY_DEFINED);
            break;
        default:
            break;
        }

    }

    return game;
}

void displayGameStats(Game* game){

    printf("Paramètres de jeu:\n");
    printf("Nombre de joueurs : %d\n", game->numberOfPlayers);
    if (game->multiplayer == 1)
        printf("Mode: multiplayer\n");
    else
        printf("Mode: solo\n");
    //printf("Nombre de bombes par joueur : %d\n", game->map[game->currentMap].initialNumberOfBombsPerPlayer);
    //printf("Dimensions de la carte: %d x %d\n", game->map[game->currentMap].sizeMapX, game->map[game->currentMap].sizeMapY);
}

void displayMapStats(Game* game){
    printf("Nombre initial de bombes par joueur : %d\n", game->map[game->currentMap].initialNumberOfBombsPerPlayer);
    printf("Dimensions de la carte: %d x %d\n", game->map[game->currentMap].sizeMapX, game->map[game->currentMap].sizeMapY);
    printf("Nombre de tunnels verticaux : %d\n", game->map[game->currentMap].numberOfVerticalTunnels);
    printf("Nombre de tunnels horizontaux : %d\n", game->map[game->currentMap].numberOfHorizontalTunnels);
    printf("\n\nAppuyez sur entrée pour continuer.\n");    
}

void displayPlayerStats(Game* game){
    short playerId = game->currentPlayer->id;
    switch (playerId)
    {
        case 1:
            colorPurple();
            break;
        case 2:
            colorRed();
            break;
        case 3:
            colorGreen();
            break;
        case 4:
            colorYellow();
            break;
    }
    ColorBold();
    printf("Tour: Joueur : %d\n", playerId);
    colorReset();
    colorCyan();
    printf("Coordonnées du joueur : x= %d y= %d\n", game->currentPlayer->x, game->currentPlayer->y);
    printf("Vie restante : %d\n", game->currentPlayer->life);
    printf("Bouclier actif : %s\n", (game->currentPlayer->shield ? "Oui" : "Non"));
    printf("Tours d'invicibilité restants : %d\n", game->currentPlayer->invincibilityTimer / game->numberOfPlayers);
    printf("Nombre de bombes restantes : %d/%d\n", game->currentPlayer->numberOfBombsLeft, game->currentPlayer->totalNumberOfBombs);
    printf("Nombre de victoires : %d\n", game->currentPlayer->numberOfVictories);
    printf("Range de la bombe : %d\n", game->currentPlayer->range);
    printf("Commandes pour jouer:\ne = ne rien faire | a = poser une bombe ( \u0E4F ) | d = \u25B6 | s = \u25BC | q = \u25C0 | z = \u25B2\n");
    colorReset();
}

Map procedurallyInitMap(Game *game, short isRandomlyDefined){

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
    if (isRandomlyDefined){
        map.sizeMapX = rand() % 39 + 11;
        map.sizeMapY = rand() % 39 + 11;
        map.numberOfHorizontalTunnels = rand() % ((map.sizeMapX - 1) / 5);
        map.numberOfVerticalTunnels = rand() % ((map.sizeMapY - 1) / 5);
        map.initialNumberOfBombsPerPlayer = 1 + rand() % 3;
    }
    else{
        while (map.initialNumberOfBombsPerPlayer < 1){
            printf("Saisir le nombre initial de bombes par joueur :");
            scanf(" %hd", &map.initialNumberOfBombsPerPlayer);
            getchar();
            if (map.initialNumberOfBombsPerPlayer < 1)
                printf("Il en faut au moins une.\n");
        }
        while (map.sizeMapX < 11 || map.sizeMapX > 51){
            printf("Saisir la hauteur de carte souhaitée (min: 11 | max: 51) :");
            scanf(" %hd", &map.sizeMapX);
            getchar();
            if (map.sizeMapX < 11)
                printf("Carte trop petite.\n");
            if (map.sizeMapX > 51)
                printf("Carte trop grande.\n");
        }
        while (map.sizeMapY < 11 || map.sizeMapY > 51){
            printf("Saisir la largeur de carte souhaitée (min: 11 | max: 51) :");
            scanf(" %hd", &map.sizeMapY);
            getchar();
            if (map.sizeMapY < 11)
                printf("Carte trop petite.\n");
            if (map.sizeMapY > 51)
                printf("Carte trop grande.\n");
        }
        do{
            printf("Saisir le nombre de tunnels horizontaux souhaité :");
            scanf(" %hd", &map.numberOfHorizontalTunnels);
            getchar();
            if (map.numberOfHorizontalTunnels > (int)((map.sizeMapX - 1) / 5))
                printf("Eh, pas trop, non plus ! C'est pas un gruyère !\n");
        } while (map.numberOfHorizontalTunnels > (int)((map.sizeMapX - 1) / 5));
        do{
            printf("Saisir le nombre de tunnels verticaux souhaité :");
            scanf(" %hd", &map.numberOfVerticalTunnels);
            getchar();
            if (map.numberOfVerticalTunnels > (int)((map.sizeMapX - 1) / 5))
                printf("Eh, pas trop, non plus ! C'est pas une éponge !\n");
        } while (map.numberOfVerticalTunnels > (int)((map.sizeMapY - 1) / 5));
    }

    // Initialize the map and its tiles
    map.maxRange = map.sizeMapX < map.sizeMapY ? (map.sizeMapX - 1) / 2 : (map.sizeMapY - 1) / 2;
    map.tile = malloc(map.sizeMapX * sizeof(Tile *));
    for (int i = 0; i < map.sizeMapX; i++){
        map.tile[i] = malloc(map.sizeMapY * sizeof(Tile));
    }
    // Center
    for (int i = 1; i < map.sizeMapX - 1; i++){
        for (int j = 1; j < map.sizeMapY - 1; j++){
            map.tile[i][j].whoIsHere = NULL;
            map.tile[i][j].whichBombIsHere = NULL;
            if (i % 2 == 0 && j % 2 == 0 && i != map.sizeMapX - 2 && j != map.sizeMapY - 2){
                if((rand() % 20) > 2)   // 15% de chance de ne rien mettre.
                    map.tile[i][j].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
                else
                    map.tile[i][j].whichItemIsHere = NULL;
            } else {
                if((rand() % 20) > 2)   // 15% de chance de ne rien mettre.
                    map.tile[i][j].whichItemIsHere = newItem(WALL);
                else
                    map.tile[i][j].whichItemIsHere = NULL;
            }
        }
    }
    // Borders
    for (int i = 0; i < map.sizeMapX; i++){
        map.tile[i][0].whoIsHere = NULL;
        map.tile[i][0].whichBombIsHere = NULL;
        map.tile[i][0].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
        map.tile[i][map.sizeMapY - 1].whoIsHere = NULL;
        map.tile[i][map.sizeMapY - 1].whichBombIsHere = NULL;
        map.tile[i][map.sizeMapY - 1].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
    }
    for (int j = 1; j < map.sizeMapY - 1; j++){
        map.tile[0][j].whoIsHere = NULL;
        map.tile[0][j].whichBombIsHere = NULL;
        map.tile[0][j].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
        map.tile[map.sizeMapX - 1][j].whoIsHere = NULL;
        map.tile[map.sizeMapX - 1][j].whichBombIsHere = NULL;
        map.tile[map.sizeMapX - 1][j].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
    }
    // Tunnels
    if (map.numberOfHorizontalTunnels){
        for (int i = 0; i < map.numberOfHorizontalTunnels; i++){
            free(map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels)][0].whichItemIsHere);
            map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels)][0].whichItemIsHere = newItem(WALL);
            free(map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels)][map.sizeMapY - 1].whichItemIsHere);
            map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels)][map.sizeMapY - 1].whichItemIsHere = newItem(WALL);
            if (rand() % 2){
                free(map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels) - 1][0].whichItemIsHere);
                map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels) - 1][0].whichItemIsHere = newItem(WALL);
                free(map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels) - 1][map.sizeMapY - 1].whichItemIsHere);
                map.tile[(int)((0.5 + i) * map.sizeMapX / map.numberOfHorizontalTunnels) - 1][map.sizeMapY - 1].whichItemIsHere = newItem(WALL);
            }
        }
    }
    if (map.numberOfVerticalTunnels){
        for (int i = 0; i < map.numberOfVerticalTunnels; i++){
            free(map.tile[0][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels)].whichItemIsHere);
            map.tile[0][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels)].whichItemIsHere = newItem(WALL);
            free(map.tile[map.sizeMapX - 1][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels)].whichItemIsHere);
            map.tile[map.sizeMapX - 1][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels)].whichItemIsHere = newItem(WALL);
            if (rand() % 2){
                free(map.tile[0][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels) - 1].whichItemIsHere);
                map.tile[0][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels) - 1].whichItemIsHere = newItem(WALL);
                free(map.tile[map.sizeMapX - 1][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels) - 1].whichItemIsHere);
                map.tile[map.sizeMapX - 1][(int)((0.5 + i) * map.sizeMapY / map.numberOfVerticalTunnels) - 1].whichItemIsHere = newItem(WALL);
            }
        }
    }
    // Players
    switch (game->numberOfPlayers){
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
        game->players[3]->totalNumberOfBombs = map.initialNumberOfBombsPerPlayer;
        game->players[3]->numberOfBombsLeft = map.initialNumberOfBombsPerPlayer;
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
        game->players[2]->totalNumberOfBombs = map.initialNumberOfBombsPerPlayer;
        game->players[2]->numberOfBombsLeft = map.initialNumberOfBombsPerPlayer;
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
        game->players[1]->totalNumberOfBombs = map.initialNumberOfBombsPerPlayer;
        game->players[1]->numberOfBombsLeft = map.initialNumberOfBombsPerPlayer;
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
        game->players[0]->totalNumberOfBombs = map.initialNumberOfBombsPerPlayer;
        game->players[0]->numberOfBombsLeft = map.initialNumberOfBombsPerPlayer;
        break;
    default:
        break;
    }

    return map;
}    // debug OK

void displayMap(Game *game){
    displayPlayerStats(game);
    for (int i = 0; i < game->map[game->currentMap].sizeMapX; ++i){
        for (int j = 0; j < game->map[game->currentMap].sizeMapY; ++j){
            if (game->map[game->currentMap].tile[i][j].whichItemIsHere != NULL){
                char tempSprite = game->map[game->currentMap].tile[i][j].whichItemIsHere->sprite;
                switch (tempSprite){
                    case 'x':
                        colorYellow();
                        printf("\u25A3");
                        colorReset();
                        break;
                    case 'm':
                        colorGreen();
                        printf("\u25A8");
                        colorReset();
                        break;
                    case 'e':
                    case '_':
                        printf(" ");
                        break;
                    default:
                        printf("%c", tempSprite);
                        break;
                }
            }
            else if (game->map[game->currentMap].tile[i][j].whoIsHere != NULL){
                short playerId;
                for(int k = 0; k != game->numberOfPlayers; k++){
                    if(game->players[k]->x == i && game->players[k]->y == j){
                        playerId = game->players[k]->id;
                    }
                }
                if(game->map[game->currentMap].tile[i][j].whichBombIsHere != NULL){
                    colorHighlight();
                }
                switch (playerId){
                    case 1:
                        colorPurple();
                        break;
                    case 2:
                        colorRed();
                        break;
                    case 3:
                        colorGreen();
                        break;
                    case 4:
                        colorYellow();
                        break;
                }
                printf("\uA66A");
                colorReset();
            }
            else if (game->map[game->currentMap].tile[i][j].whichBombIsHere != NULL){
                printf("\u0E4F");
            }
            else
                printf(" ");
        }
        printf("\n");
    }
}

void deleteTile(Tile *tile){
    free(tile->whichItemIsHere);
    free(tile->whichBombIsHere);
    free(tile->whoIsHere);
    free(tile);
}

void deleteMap(Game *game){
    for (int i = 0; i < game->map[game->currentMap].sizeMapX; i++){
        for (int j = 0; j < game->map[game->currentMap].sizeMapY; j++){
            deleteTile(&game->map[game->currentMap].tile[i][j]);
        }

        free(game->map[game->currentMap].tile[i]);
    }
    free(game->map[game->currentMap].tile);
}

void deleteGame(Game *game){
    deleteMap(game);
    deletePlayers(game->players, game->numberOfPlayers);
    while (game->activeBombs != NULL){
        BombList *temp = game->activeBombs;
        game->activeBombs = game->activeBombs->nextOne;
        free(temp);
    }
}

// detect key pressed
char keypress(){
    system("/bin/stty raw");
    int c;
    system("/bin/stty -echo");
    c = getc(stdin);
    system("/bin/stty echo");
    system("/bin/stty cooked");

    switch (c){
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

void clearScreen(){
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

void bombKick(Game *game, short direction){
    Bomb *kickMe;
    Tile *originTile, *destinationTile, *nextTile;
    Map* map = &game->map[game->currentMap];
    int howFarItGoes = 0;

    originTile = &game->map[game->currentMap].tile[game->currentPlayer->x][game->currentPlayer->y];
    nextTile = originTile;
    kickMe = originTile->whichBombIsHere;

    switch (direction){
    case XMINUS:
        do{
            destinationTile = nextTile;
            howFarItGoes++;
            nextTile = &game->map[game->currentMap].tile[((game->currentPlayer->x - howFarItGoes) + map->sizeMapX) % game->map[game->currentMap].sizeMapX][game->currentPlayer->y];
        } while (nextTile->whichBombIsHere == NULL && nextTile->whichItemIsHere == NULL && nextTile->whoIsHere == NULL);
        break;
    case XPLUS:
        do{
            destinationTile = nextTile;
            howFarItGoes++;
            nextTile = &game->map[game->currentMap].tile[((game->currentPlayer->x + howFarItGoes) + map->sizeMapX) % game->map[game->currentMap].sizeMapX][game->currentPlayer->y];
        } while (nextTile->whichBombIsHere == NULL && nextTile->whichItemIsHere == NULL && nextTile->whoIsHere == NULL);
        break;
    case YMINUS:
        do{
            destinationTile = nextTile;
            howFarItGoes++;
            nextTile = &game->map[game->currentMap].tile[game->currentPlayer->x][((game->currentPlayer->y - howFarItGoes) + map->sizeMapY) % game->map[game->currentMap].sizeMapY];
        } while (nextTile->whichBombIsHere == NULL && nextTile->whichItemIsHere == NULL && nextTile->whoIsHere == NULL);
        break;
    case YPLUS:
        do{
            destinationTile = nextTile;
            howFarItGoes++;
            nextTile = &game->map[game->currentMap].tile[game->currentPlayer->x][((game->currentPlayer->y + howFarItGoes) + map->sizeMapY) % game->map[game->currentMap].sizeMapY];
        } while (nextTile->whichBombIsHere == NULL && nextTile->whichItemIsHere == NULL && nextTile->whoIsHere == NULL);
        break;
    default:
        break;
    }
    destinationTile->whichBombIsHere = kickMe;
    originTile->whichBombIsHere = NULL;
}

short getPlayerAction(){
    char key = keypress();
    switch (key){
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

void botAction(Game* game){
    // Pas le temps de l'implémenter, mais j'ai toute l'IA conçue sur papier.
    // Je vais juste mettre les grande lignes et bricoler un truc en 20mn.
    // J'ai voulu faire un "botAction", mais quand j'ai vu l'heure, je me suis dit "botA... Nique, j'ai pas le temps !"

    // check if puts a bomb : for each tile that a bomb could touch, check if there is another player at 2 tiles or less.
    //          If yes, put a bomb.
    //          If no, check if laying a bomb would destroy a wall.
    //              If yes, lay a bomb.

    // check if move :
    //      Check if tile is dangerous.
    //          If yes, check safetiness of adjacent tile.
    //              If multiple adjacent safe tiles, move forward if has bombs left, otherwise move away.
    //          If no, for each tile that a bomb could touch, check if there is another player at 2 tiles or less.
    //              If yes, put a bomb.
    //              If no, check if laying a bomb would destroy a wall.
    //                  If yes, lay a bomb.
    //                  If no, check there is safe adjacent tiles.
    //                      If yes, do a A* to check if there is a way to a target, with a distance limit (maybe range + 2 tiles ?).
    //                          If yes, move forward if has bombs left, otherwise move away.
    //                          If no, go towards the nearest Item (search the nearest tiles, then expand the search up to a distance (maybe 4 to 5 tiles), kinda like a reverted A*).
    //                              If no Item in range, move randomly (any direction or don't move).
    //                      If no, don't move.

/*
    
    


#include <math.h>
    short nearestPlayersId = -1;
    short directionToNearestPlayer = DONT_MOVE;
    double distanceToNearestPlayer = -1;
    short coordinatesDifferenceToPlayer = malloc(game->numberOfPlayers * 2 *sizeof(short));
    double distanceToPlayer = malloc(game->numberOfPlayers * sizeof(double));
    for (int i = 0; i < game->numberOfPlayers; i++){
        distanceToPlayer[i] = 0;
    }

    for (int i = 1; i < map->sizeMapX; i++){
        for (int j = -i; j < i; j++){
            if (j < -map->sizeMapY / 2)
                continue;
            if (j >= map->sizeMapY / 2)
                break;
            Player* enemy = map->tile[(mover->x + i + map->sizeMapX) % map->sizeMapX][(mover->x + j + map->sizeMapY) % map->sizeMapY]->whoIsHere;
            if (enemy != NULL){
                short enemyId = enemy->id - 1;
                coordinatesDifferenceToPlayer[enemyId * 2] = enemy->x;
                coordinatesDifferenceToPlayer[enemyId * 2 + 1] = enemy->y;
                distanceToPlayer[enemyId] =  pow(pow((coordinatesDifferenceToPlayer[enemyId * 2] - mover->x + map->sizeMapX) % map->sizeMapX, 2)
                                                 + pow((coordinatesDifferenceToPlayer[enemyId * 2 + 1] - mover->y + map->sizeMapY) % map->sizeMapX, 2), 0.5);
                if (distanceToNearestPlayer && distanceToPlayer < distanceToPlayer[enemyId]){
                    distanceToPlayer = distanceToPlayer[enemyId];
                    nearestPlayersId = enemyId;
                    directionToNearestplayer = XPLUS;
                }
            }
        }
    }
    for (int i = 1; i < map->sizeMapX; i++){
        for (int j = -i; j < i; j++){
            if (j < -map->sizeMapY / 2)
                continue;
            if (j >= map->sizeMapY / 2)
                break;
            Player* enemy = map->tile[(mover->x + j + map->sizeMapX) % map->sizeMapX][(mover->x + i + map->sizeMapY) % map->sizeMapY]->whoIsHere;
            if (enemy != NULL){
                short enemyId = enemy->id - 1;
                coordinatesDifferenceToPlayer[enemyId * 2] = enemy->x;
                coordinatesDifferenceToPlayer[enemyId * 2 + 1] = enemy->y;
                distanceToPlayer[enemyId] =  pow(pow((coordinatesDifferenceToPlayer[enemyId * 2] - mover->x + map->sizeMapX) % map->sizeMapX, 2)
                                                 + pow((coordinatesDifferenceToPlayer[enemyId * 2 + 1] - mover->y + map->sizeMapY) % map->sizeMapX, 2), 0.5);
                if (distanceToNearestPlayer && distanceToPlayer < distanceToPlayer[enemyId]){
                    distanceToPlayer = distanceToPlayer[enemyId];
                    nearestPlayersId = enemyId;
                    directionToNearestplayer = YPLUS;
                }
            }
        }
    }
    for (int i = 1; i < map->sizeMapX; i++){
        for (int j = -i; j < i; j++){
            if (j < -map->sizeMapY / 2)
                continue;
            if (j >= map->sizeMapY / 2)
                break;
            Player* enemy = map->tile[(mover->x - i + map->sizeMapX) % map->sizeMapX][(mover->x - j + map->sizeMapY) % map->sizeMapY]->whoIsHere;
            if (enemy != NULL){
                short enemyId = enemy->id - 1;
                coordinatesDifferenceToPlayer[enemyId * 2] = enemy->x;
                coordinatesDifferenceToPlayer[enemyId * 2 + 1] = enemy->y;
                distanceToPlayer[enemyId] =  pow(pow((coordinatesDifferenceToPlayer[enemyId * 2] - mover->x + map->sizeMapX) % map->sizeMapX, 2)
                                                 + pow((coordinatesDifferenceToPlayer[enemyId * 2 + 1] - mover->y + map->sizeMapY) % map->sizeMapX, 2), 0.5);
                if (distanceToNearestPlayer && distanceToPlayer < distanceToPlayer[enemyId]){
                    distanceToPlayer = distanceToPlayer[enemyId];
                    nearestPlayersId = enemyId;
                    directionToNearestplayer = XMINUS;
                }
            }
        }
    }
    for (int i = 1; i < map->sizeMapX; i++){
        for (int j = -i; j < i; j++){
            if (j < -map->sizeMapY / 2)
                continue;
            if (j >= map->sizeMapY / 2)
                break;
            Player* enemy = map->tile[(mover->x - j + map->sizeMapX) % map->sizeMapX][(mover->x - i + map->sizeMapY) % map->sizeMapY]->whoIsHere;
            if (enemy != NULL){
                short enemyId = enemy->id - 1;
                coordinatesDifferenceToPlayer[enemyId * 2] = enemy->x;
                coordinatesDifferenceToPlayer[enemyId * 2 + 1] = enemy->y;
                distanceToPlayer[enemyId] =  pow(pow((coordinatesDifferenceToPlayer[enemyId * 2] - mover->x + map->sizeMapX) % map->sizeMapX, 2)
                                                 + pow((coordinatesDifferenceToPlayer[enemyId * 2 + 1] - mover->y + map->sizeMapY) % map->sizeMapX, 2), 0.5);
                if (distanceToNearestPlayer && distanceToPlayer < distanceToPlayer[enemyId]){
                    distanceToPlayer = distanceToPlayer[enemyId];
                    nearestPlayersId = enemyId;
                    directionToNearestplayer = XPLUS;
                }
            }
        }
    }




    short **dangerMap = malloc(map->sizeMapX * sizeof(short *));
    for (int i = 0; i < map->sizeMapX; i++){
        dangerMap[i] = malloc(map->sizeMapY * sizeof(short));
        for (int j = 0; j < map->sizeMapY; j++){
            dangerMap[i][j] = 0;
        }
    }
    BombList* node = game->activeBombs;
    while (node != NULL){
        Bomb* bombTocheck = node->thisBomb;
        if (bombToCheck->timer > 1)
            break;

        // The 4 next loops are just one done for each direction.
        // The flame of the explosion goes up to one wall (and destructs it if able) or up to range, whichever the less.
        // It sets off other bombs it touches and destroys all items in its path.
        int i = 1;
        while (i <= bombTocheck->range){
            Item *currentItem = map->tile[(bombTocheck->x + i) % map->sizeMapX][bombTocheck->y].whichItemIsHere;
            if (dangerMap[(bombTocheck->x + i) % map->sizeMapX][bombTocheck->y] == 1 && !(currentItem != NULL && (currentItem->ID == WALL || currentItem->ID == INDESTRUCTIBLE_WALL))){
                i++;
                continue;
            }
            else{
                if (currentItem != NULL && (currentItem->ID == INDESTRUCTIBLE_WALL || currenItem->ID == WALL)){
                    dangerMap[(bombTocheck->x + i) % map->sizeMapX][bombTocheck->y] = 1;
                    break;
                }
            }
            i++;
        }

        i = 1;
        while (i <= bombTocheck->range){
            Item *currentItem = map->tile[bombTocheck->x][(bombTocheck->y + i) % map->sizeMapY].whichItemIsHere;
            if (dangerMap[bombTocheck->x][(bombTocheck->y + i) % map->sizeMapY] == 1 && !(currentItem != NULL && (currentItem->ID == WALL || currentItem->ID == INDESTRUCTIBLE_WALL))){
                i++;
                continue;
            }
            if (currentItem != NULL && (currentItem->ID == INDESTRUCTIBLE_WALL || currenItem->ID == WALL)){
                dangerMap[bombTocheck->x][(bombTocheck->y + i) % map->sizeMapY] = 1;
                break;
            }
            i++;
        }

        i = 1;
        while (i <= bombTocheck->range){
            Item *currentItem = map->tile[(bombTocheck->x - i) % map->sizeMapX][bombTocheck->y].whichItemIsHere;
            if (dangerMap[(bombTocheck->x - i) % map->sizeMapX][bombTocheck->y] == 1 && !(currentItem != NULL && (currentItem->ID == WALL || currentItem->ID == INDESTRUCTIBLE_WALL))){
                i++;
                continue;
            }
            if (currentItem != NULL && (currentItem->ID == INDESTRUCTIBLE_WALL || currenItem->ID == WALL)){
                dangerMap[(bombTocheck->x - i) % map->sizeMapX][bombTocheck->y] = 1;
                break;
            }
            i++;
        }

        i = 1;
        while (i <= bombTocheck->range){
            Item *currentItem = map->tile[bombTocheck->x][(bombTocheck->y - i) % map->sizeMapY].whichItemIsHere;
            if (dangerMap[bombTocheck->x][(bombTocheck->y - i) % map->sizeMapY] == 1 && !(currentItem != NULL && (currentItem->ID == WALL || currentItem->ID == INDESTRUCTIBLE_WALL))){
                i++;
                continue;
            }
            if (currentItem != NULL && (currentItem->ID == INDESTRUCTIBLE_WALL || currenItem->ID == WALL)){
                dangerMap[bombTocheck->x + i][(bombTocheck->y - i) % map->sizeMapY] = 1;
                break;
            }
            i++;
        }

        node = node->nextOne;
    }
    if (dangerMap[mover->x][mover->y]){
        short directionsPossible = 4;
        short directionSafety[4];
        directionsPossible -= (dangerMap[(mover->x + 1) % game->map[game->currentMap]->sizeMapX][mover->y]);
        directionSafety[0] = (dangerMap[(mover->x + 1) % game->map[game->currentMap]->sizeMapX][mover->y]);
        directionsPossible -= (dangerMap[mover->x][(mover->y + 1) % game->map[game->currentMap]->sizeMapY]);
        directionSafety[1] = (dangerMap[mover->x][(mover->y + 1) % game->map[game->currentMap]->sizeMapY]);
        directionsPossible -= (dangerMap[(mover->x + 1) % game->map[game->currentMap]->sizeMapX][mover->y]);
        directionSafety[2] = (dangerMap[(mover->x + 1) % game->map[game->currentMap]->sizeMapX][mover->y]);
        directionsPossible -= (dangerMap[mover->x][(mover->y - 1) % game->map[game->currentMap]->sizeMapY]);
        directionSafety[3] = (dangerMap[mover->x][(mover->y - 1) % game->map[game->currentMap]->sizeMapY]);
        short whereToGo = 0;
        if (directionsPossible){

        }
    }


    // Sur la version rendue, c'est juste juste du random pur.
    Map *map = &game->map[game->currentMap];
    Player *mover = game->currentPlayer;

    short currentX = mover->x, currentY = mover->y, destinationX = mover->x, destinationY = mover->y;
    short direction = MOVEMENT_KEY_ERROR;
    do{
        direction = rand() % 6;
    } while (direction == MOVEMENT_KEY_ERROR || (map->tile[currentX][currentY].whichBombIsHere != NULL && direction == PUT_BOMB));

    switch (direction){
        case PUT_BOMB:
            if (mover->numberOfBombsLeft){
                Bomb *whichBomb = newBomb(game);
                map->tile[destinationX][destinationY].whichBombIsHere = whichBomb;
                mover->numberOfBombsLeft--;
            }
            return;
        case DONT_MOVE:
            return;
        case XPLUS:
            destinationX = ((destinationX + 1) + map->sizeMapX) % map->sizeMapX;
            break;
        case XMINUS:
            destinationX = ((destinationX - 1) + map->sizeMapX) % map->sizeMapX;
            break;
        case YPLUS:
            destinationY = ((destinationY + 1) + map->sizeMapY) % map->sizeMapY;
            break;
        case YMINUS:
            destinationY = ((destinationY - 1) + map->sizeMapY) % map->sizeMapY;
            break;
        default:
            return;
    }
    printf("%d, %d\n", destinationX, destinationY);

    Item *whichItemIsHere = map->tile[destinationX][destinationY].whichItemIsHere;
    Bomb *whichBombIsHere = map->tile[destinationX][destinationY].whichBombIsHere;
    Player *whoIsHere = map->tile[destinationX][destinationY].whoIsHere;
    if (whoIsHere != NULL){
        return;
    }
    if (whichItemIsHere != NULL){

        switch (whichItemIsHere->ID){
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
    else if (whichBombIsHere != NULL){
        switch (mover->interactionWithBombs){
            case PIETON:
                return;
            case BOMB_WALK:
            mover->x = destinationX;
            mover->y = destinationY;
            map->tile[currentX][currentY].whoIsHere = NULL;
            map->tile[destinationX][destinationY].whoIsHere = mover;
                return;
            case BOMB_KICKING:
                bombKick(game, direction);
                return;
        }
    }
    else {
        mover->x = destinationX;
        mover->y = destinationY;
        map->tile[currentX][currentY].whoIsHere = NULL;
        map->tile[destinationX][destinationY].whoIsHere = mover;
        return;
    }
}

void playerAction(Game *game){
    Map *map = &game->map[game->currentMap];
    Player *mover = game->currentPlayer;
    if (!mover->life)
        return;
    if (!mover->isHuman){
        botAction(game);
        return;
    }
    short currentX = mover->x, currentY = mover->y, destinationX = mover->x, destinationY = mover->y;
    short direction = MOVEMENT_KEY_ERROR;
    do{
        direction = getPlayerAction();
        printf("%d\n", direction);
    } while (direction == MOVEMENT_KEY_ERROR || (map->tile[currentX][currentY].whichBombIsHere != NULL && direction == PUT_BOMB));
    switch (direction){
    case PUT_BOMB:
        if (mover->numberOfBombsLeft){
            Bomb *whichBomb = newBomb(game);
            map->tile[destinationX][destinationY].whichBombIsHere = whichBomb;
            mover->numberOfBombsLeft--;
        }
        return;
    case DONT_MOVE:
        return;
    case XPLUS:
        destinationX = ((destinationX + 1) + map->sizeMapX) % map->sizeMapX;
        break;
    case XMINUS:
        destinationX = ((destinationX - 1) + map->sizeMapX) % map->sizeMapX;
        break;
    case YPLUS:
        destinationY = ((destinationY + 1) + map->sizeMapY) % map->sizeMapY;
        break;
    case YMINUS:
        destinationY = ((destinationY - 1) + map->sizeMapY) % map->sizeMapY;
        break;
    default:
        return;
    }
    printf("%d, %d\n", destinationX, destinationY);

    Item *whichItemIsHere = map->tile[destinationX][destinationY].whichItemIsHere;
    Bomb *whichBombIsHere = map->tile[destinationX][destinationY].whichBombIsHere;
    Player *whoIsHere = map->tile[destinationX][destinationY].whoIsHere;
    if (whoIsHere != NULL){
        return;
    }
    if (whichItemIsHere != NULL){

        switch (whichItemIsHere->ID){
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
    else if (whichBombIsHere != NULL){
        switch (mover->interactionWithBombs){
        case PIETON:
            return;
        case BOMB_WALK:
            mover->x = destinationX;
            mover->y = destinationY;
            map->tile[currentX][currentY].whoIsHere = NULL;
            map->tile[destinationX][destinationY].whoIsHere = mover;
            return;
        case BOMB_KICKING:
            bombKick(game, direction);
            return;
        }
    }
    else {
        mover->x = destinationX;
        mover->y = destinationY;
        map->tile[currentX][currentY].whoIsHere = NULL;
        map->tile[destinationX][destinationY].whoIsHere = mover;
        return;
    }
}

void hitPlayer(Player *dommageCollateral, Game *game){
    if (dommageCollateral->invincibilityTimer)
        return;
    if (dommageCollateral->shield){
        dommageCollateral->shield = 0;
        return;
    }
    dommageCollateral->life--;
    if (!dommageCollateral->life){
        clearScreen();
        printf("Rest in pieces, player %d.\n", dommageCollateral->id);
        sleep(4);
        game->map[game->currentMap].tile[dommageCollateral->x][dommageCollateral->y].whoIsHere = NULL;
        game->numberOfAlivePlayers--;
        // Maybe add something so the game knows that player is dead ? Not really needed, but, eh, why not ?
    }
}

void nextMap(Game* game){
    game->currentMap++;
    game->playerTurn = 0;
    game->currentPlayer = game->players[0];
    game->numberOfAlivePlayers = game->numberOfPlayers;
    switch (game->numberOfPlayers){
        case 4:
            game->players[3]->range = 1;
            game->players[3]->shield = 0;
            game->players[3]->invincibilityTimer = 0;
            game->players[3]->x = 1;
            game->players[3]->y = game->map[game->currentMap].sizeMapY - 2;
            game->players[3]->totalNumberOfBombs = game->map[game->currentMap].initialNumberOfBombsPerPlayer;
            game->players[3]->numberOfBombsLeft = game->map[game->currentMap].initialNumberOfBombsPerPlayer;
            game->players[3]->interactionWithBombs = NOTHING;
        case 3:
            game->players[2]->range = 1;
            game->players[2]->shield = 0;
            game->players[2]->invincibilityTimer = 0;
            game->players[2]->x = game->map[game->currentMap].sizeMapX - 2;
            game->players[2]->y = 1;
            game->players[2]->totalNumberOfBombs = game->map[game->currentMap].initialNumberOfBombsPerPlayer;
            game->players[2]->numberOfBombsLeft = game->map[game->currentMap].initialNumberOfBombsPerPlayer;
            game->players[2]->interactionWithBombs = NOTHING;
        case 2:
            game->players[1]->range = 1;
            game->players[1]->shield = 0;
            game->players[1]->invincibilityTimer = 0;
            game->players[1]->x = game->map[game->currentMap].sizeMapX - 2;
            game->players[1]->y = game->map[game->currentMap].sizeMapY - 2;
            game->players[1]->totalNumberOfBombs = game->map[game->currentMap].initialNumberOfBombsPerPlayer;
            game->players[1]->numberOfBombsLeft = game->map[game->currentMap].initialNumberOfBombsPerPlayer;
            game->players[1]->interactionWithBombs = NOTHING;
        case 1:
            game->players[1]->range = 1;
            game->players[1]->shield = 0;
            game->players[1]->invincibilityTimer = 0;
            game->players[0]->x = 1;
            game->players[0]->y = 1;
            game->players[0]->totalNumberOfBombs = game->map[game->currentMap].initialNumberOfBombsPerPlayer;
            game->players[0]->numberOfBombsLeft = game->map[game->currentMap].initialNumberOfBombsPerPlayer;
            game->players[0]->interactionWithBombs = NOTHING;
            break;
        default:
            break;
    }
    // to be completed
    // don't forget to first treat the case where there is no next Map
/**/}


Map initMapFromFile(Game *game, char *mapName){
    Map map;
    FILE *mapFile = fopen(mapName, "r");
    if (mapFile == NULL)
    {
        printf("Error : couldn't open map file %s", mapName);
        exit(1);
    }
    char *line = malloc(sizeof(char) * 100);
    fgets(line, 100, mapFile);
    map.sizeMapX = atoi(strtok(line, " "));
    map.sizeMapY = atoi(strtok(NULL, " "));
    map.tile = malloc(sizeof(Tile *) * map.sizeMapX);

    for (int i = 0; i < map.sizeMapX; i++)
    {
        map.tile[i] = malloc(sizeof(Tile) * map.sizeMapY);
        for (int j = 0; j < map.sizeMapY; j++)
        {
            map.tile[i][j].whichBombIsHere = NULL;
            map.tile[i][j].whichItemIsHere = NULL;
            map.tile[i][j].whoIsHere = NULL;
        }
    }
    short idPlayerToPlace = 0;
    for (int i = 0; i < map.sizeMapX; i++){
        fgets(line, 100, mapFile);
        for (int j = 0; j < map.sizeMapY; j++)
        {
            switch (line[j])
            {
                case 'x':
                    map.tile[i][j].whichItemIsHere = newItem(INDESTRUCTIBLE_WALL);
                    break;
                case 'p':
                    if (idPlayerToPlace <= game->numberOfPlayers){
                        map.tile[i][j].whoIsHere = game->players[idPlayerToPlace];
                        map.tile[i][j].whoIsHere->x = i;
                        map.tile[i][j].whoIsHere->y = j;
                        idPlayerToPlace++;
                    }
                    break;
                case 'm':
                    map.tile[i][j].whichItemIsHere = newItem(WALL);
                    break;
                case 'e':
                case '_':
                    map.tile[i][j].whichItemIsHere = newItem(NOTHING);
                    break;
                default:
                    break;
            }
        }
    }
    fclose(mapFile);
    return map;
}
