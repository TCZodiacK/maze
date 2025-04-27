#include <stdio.h>
#include <stdlib.h>

/*
 * TO DO LIST
 * IMPLEMENT GAME LOGIC FUNCTION (BIGGEST ASPECT OF THE GAME REMAINING)
 * IMPLEMENT SCREEN AFTER PLAYER WINS THE GAME
 * IMPLEMENT CUSTOM MAP IMPORTING
 * IMPLEMENT CUSTOM MAP GAMEPLAY
 * IMPLEMENT VALIDATION AND DISPLAY APPROPRIATE ERRORS IF ANY FUNCTION FAILS FOR SOME REASON
 * IMPLEMENT ADDITIONAL OPTIONAL PLAYER TRACKING DATA LIKE TURNS TAKEN, ETC.
 * MAKE MENUS MORE PRETTY, MAYBE EVEN DESIGN ASCII ART OF A GAME TITLE OR SOMETHING FOR THE MAIN MENU
 */

#define MAXROWS 22
#define MAXCOLS 22
#define MAXLEVEL 2

struct data {
    char name[10];
    int level;
    int deaths;
};

void display(int maze[MAXROWS][MAXCOLS], char key[], int prow, int pcol);
int initialize(int maze[MAXLEVEL][MAXROWS][MAXCOLS], int playerstart[MAXLEVEL][2]);
int getnewplayerdata(struct data *player);
int getsavedata(struct data *player);
int savedata(struct data player);
int startgame(int maze[MAXLEVEL][MAXROWS][MAXCOLS], int playerstart[MAXLEVEL][2], char key[], struct data *player);
int preparelevel(int level, int maze[MAXLEVEL][MAXROWS][MAXCOLS]);
int playlevel(int maze[MAXROWS][MAXCOLS], int prow, int pcol, char key[], struct data *player);
int getmove(char key[]);
int gamelogic(int maze[MAXROWS][MAXCOLS], int move, int *prow, int *pcol, struct data *player);


int main(void) {
    /*
     *key values:
     *0-empty
     *1-wall
     *2-objective space
     *3-key
     *4-gate
     *5-spike trap/hole
     *6-ice
     *7-teleporter
    */
    //ascii values to be displayed for each tile type
    char key[] = {' ', 'X', '!', 'K', 'G', 'O', 'I', 'T'};
    //starting location of the player in each level. listed as row then column
    int playerstart[MAXLEVEL + 2][2];
    //layouts of the mazes, will be filled in by pulling info from a text file
    int maze[MAXLEVEL + 2][MAXROWS][MAXCOLS] = {0};
    initialize (maze, playerstart);
    int menuchoice = -1;

    struct data player;

    while (menuchoice != 5) {
        printf("\t\t\t\t\t    \033[33mWelcome to Maze\n");
        printf("\t\t\t\t\t  1. New Game\n");
        printf("\t\t\t\t\t  2. Continue\n");
        printf("\t\t\t\t\t  3. Import Custom Map\n");
        printf("\t\t\t\t\t  4. Play Custom Map\n");
        printf("\t\t\t\t\t  5. Exit\n");
        printf("\t\t\t\t\t  Enter your choice: \033[0m");
        scanf("%d", &menuchoice);
        switch (menuchoice) {
            case 1:
                getnewplayerdata(&player);
                startgame(maze, playerstart, key, &player);
            break;

            case 2:
                getsavedata(&player);
                startgame(maze, playerstart, key, &player);
            break;

            case 3:
                printf("import custom maze\n");
            break;

            case 4:
                printf("play custom maze\n");
            break;

            case 5:
            break;

            default:
                printf("Invalid choice\n");
            break;
        }
    }

    return 0;
}

void display(int maze[MAXROWS][MAXCOLS], char key[], int prow, int pcol) {
    //this function will display the current configuration to the player
    //printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    for (int i = 0; i < MAXROWS; i++) {
        if (maze[i][0] == 0) {
            printf("\n");
            return;
        }
        for (int j = 0; j < MAXCOLS; j++) {
            if (i == prow && j == pcol) {
                printf("P");
            }
            else {
                printf("%c", key[maze[i][j]]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int initialize(int maze[MAXLEVEL][MAXROWS][MAXCOLS], int playerstart[MAXLEVEL][2]) {
    //this function should read the maze.txt file to get the data for all the levels in the game, and save that data in the appropriate arrays

    int rows, cols, i, j;
    FILE* ifp = fopen("maze.txt", "r");
    if (ifp == NULL) {
        printf("Error: Could not open file.\n");
        return -1;
    }
    for (int layer = 1; layer <= MAXLEVEL; layer++) {
        if (fscanf(ifp, "%d %d %d %d", &rows, &cols, &playerstart[layer][0], &playerstart[layer][1]) != 4) {
            printf("Error: Corrupted Data File\n");
            fclose(ifp);
            return -1;
        }
        if (rows > MAXROWS - 2 || cols > MAXCOLS - 2) {
            printf("Error: Invalid Map Size\n");
            fclose(ifp);
            return -1;
        }
        for (j = 0; j < cols + 2; j++) {
            maze[layer][0][j] = 1;
        }
        for (i = 1; i <= rows; i++) {
            maze[layer][i][0] = 1;
            for (j = 1; j <= cols; j++) {
                fscanf(ifp, "%d", &maze[layer][i][j]);
            }
            maze[layer][i][j] = 1;
        }
        for (j = 0; j < cols + 2; j++) {
            maze[layer][i][j] = 1;
        }
    }
    fclose(ifp);
    return 0;
}

int getnewplayerdata(struct data *player) {
    //this function will get the new player data from the user
    printf("Enter new your name: ");
    scanf("%s", player->name);
    player->level = 1;
    player->deaths = 0;
    savedata(*player);
    return 0;
}

int getsavedata(struct data *player) {
    //this function will get the saved data from a returning player
    FILE* ifp = fopen("save.txt", "r");
    if (ifp == NULL) {
        printf("Error: Could not open save file.\n");
        return -1;
    }
    fscanf(ifp, "%d", &player->level);
    fscanf(ifp, "%d", &player->deaths);
    fscanf(ifp, "%s", &player->name);
    return 0;
}

int savedata(struct data player) {
    //this function will save the players current data to their save file
    FILE* ofp = fopen("save.txt", "w");
    if (ofp == NULL) {
        printf("Error: Could not open save file.\n");
        return -1;
    }
    fprintf(ofp, "%d\n", player.level);
    fprintf(ofp, "%d\n", player.deaths);
    fprintf(ofp, "%s\n\n", player.name);
    fclose(ofp);
    return 0;
}

int startgame(int maze[MAXLEVEL][MAXROWS][MAXCOLS], int playerstart[MAXLEVEL][2], char key[], struct data *player) {
    //this function is the function that will manage the player between levels and load appropriate levels

    int playing = 1;
    while (playing) {
        preparelevel(player->level, maze);
        playlevel(maze[0], playerstart[player->level][0], playerstart[player->level][1], key, player);
        printf("Your current level: %d\nWould you like to keep playing?\nEnter 1 for yes, or 0 for no: ", player->level);
        scanf("%d", &playing);
    }
}

int preparelevel(int level, int maze[MAXLEVEL][MAXROWS][MAXCOLS]) {
    //this function will load the proper level data into the active level slot, layer 0  of the maze array

    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            maze[0][i][j] = maze[level][i][j];
        }
    }
}

int playlevel(int maze[MAXROWS][MAXCOLS], int prow, int pcol, char key[], struct data *player) {
    //this function will run through the turn by turn actions of the level itself, ie display screen, get player move, determine what happens, update player
    int playing_level = 1;
    while (playing_level) {
        display(maze, key, prow, pcol);
        int move = getmove(key);
        playing_level = gamelogic(maze, move, &prow, &pcol, player);
    }
}

int getmove(char key[]) {
    char move;
    printf("\nEnter W, A, S, or D to choose your direction, F to forfeit the level, or H for help, then press enter: ");
    scanf(" %c", &move);
    switch (move) {
        case 'W':
            case 'w':
        return 1;

        case 'A':
            case 'a':
        return 2;

        case 'S':
            case 's':
        return 3;

        case 'D':
            case 'd':
        return 4;

        case 'F':
            case 'f':
        return 5;

        case 'H':
            case 'h':
        printf("P: Player, %c: Wall, %c: Goal, %c: Key, %c: Gate, %c: Pitfall, %c: Ice, %c: Teleporter\n", key[1], key[2], key[3], key[4], key[5], key[6], key[7]);
        return getmove(key);

        default: return 0;
    }
}

int gamelogic(int maze[MAXROWS][MAXCOLS], int move, int *prow, int *pcol, struct data *player) {
    //if the player chose to forfeit
    if (move == 5) {
        player->deaths++;
        return 0;
    }

    //target is going to store the row and column the player is trying to move to
    int target[2];
    //target type will be the tile type the player is trying to move onto
    int targettype;

    //these lines look super funky, but basically if they choose 2 or 4, they will move left or right, 1 or 3 will move up or down
    if (move % 2) {
        //we can subtract 3 from 2 or 4 to get -1 or +1 and add that to the appropriate player coordinates to find the target tile
        target[0] = *prow;
        target[1] = *pcol + move - 3;
    }
    else {
        //we can subtract 2 from 1 or 3 to get -1 or +1 and add that to the appropriate player coordinates to find the target tile
        target[0] = *prow + move - 2;
        target[1] = *pcol;
    }


    targettype = maze[target[0]][target[1]];
    switch (targettype) {
        case 0:

        break;
        case 1:

        break;
        case 2:

        break;
        case 3:

        break;
        case 4:

        break;
        case 5:

        break;
        case 6:

        break;
        case 7:

        break;
    }

}
