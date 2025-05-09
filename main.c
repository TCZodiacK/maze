//BSCS 1B
/*
 *Game Designers
 *Aaron Beard
 *Chris Justin Taer
 *Rens Layco Moreno
 *Cloud Daet
 *John Andrew Lonceras
 */
#include <stdio.h>

#define MAXROWS 22
#define MAXCOLS 22
#define MAXLEVEL 9

//this structure will save the relevant player data, and also keep track of variables that need to be passed between different functions
struct data {
    char name[10];
    int level;
    int deaths;
    int moves;
    int row;
    int col;
    int targetrow;
    int targetcol;
    int keys;
    int custom_map_found;
};

void display(int maze[MAXROWS][MAXCOLS], char key[], int key_color[], struct data player);
int initialize(int maze[MAXLEVEL][MAXROWS][MAXCOLS], int playerstart[MAXLEVEL][2], struct data *player);
int getnewplayerdata(struct data *player);
int getsavedata(struct data *player);
int savedata(struct data player);
void startgame(int maze[MAXLEVEL][MAXROWS][MAXCOLS], int playerstart[MAXLEVEL][2], char key[], int key_color[], struct data *player, int custom);
void preparelevel(int level, int maze[MAXLEVEL][MAXROWS][MAXCOLS], int playerstart[MAXLEVEL][2], struct data *player);
void playlevel(int maze[MAXROWS][MAXCOLS], char key[], int key_color[], struct data *player, int custom, int levelplayed);
int getmove();
int gamelogic(int maze[MAXROWS][MAXCOLS], int move, struct data *player);
void custominstructions(int key_color[]);


int main(void) {
    /*
     *key values:
     *0-empty
     *1-wall
     *2-objective space
     *3-key
     *4-door
     *5-spike trap/hole
     *6-ice
     *7-teleporter
    */

    struct data player;
    int savelocated = getsavedata(&player);
    player.custom_map_found = 0;

    //ascii values to be displayed for each tile type
    char key[] = {' ', '#', '!', 'K', 'D', 'O', 'I', 'T'};
    int key_color[] = {0, 7, 3, 2, 2, 1, 6, 5};
    //starting location of the player in each level. listed as row then column
    int playerstart[MAXLEVEL + 2][2];
    //layouts of the mazes, will be filled in by pulling info from a text file
    int maze[MAXLEVEL + 2][MAXROWS][MAXCOLS] = {0};
    initialize (maze, playerstart, &player);
    char menuchoice = '0';

    while (menuchoice != '7') {
        printf("\t\t\t\t\t     \033[33mMain Menu\n\n");
        printf("\t\t\t\t\t  1. New Game\n");
        printf("\t\t\t\t\t  2. Continue\n");
        printf("\t\t\t\t\t  3. Play Custom Map\n");
        printf("\t\t\t\t\t  4. Custom Map Upload Instructions\n");
        printf("\t\t\t\t\t  5. View Player Data\n");
        printf("\t\t\t\t\t  6. View Credits\n");
        printf("\t\t\t\t\t  7. Exit\n\n");
        printf("\t\t\t\t\t     Enter your choice: \033[0m");
        scanf(" %c", &menuchoice);
        switch (menuchoice) {
            case '1':
                getnewplayerdata(&player);
                startgame(maze, playerstart, key, key_color, &player, 0);
            break;

            case '2':
                if (savelocated == -1) {
                    printf("\033[31mNo save data found\n\033[0m");
                }
                else {
                    startgame(maze, playerstart, key, key_color, &player, 0);
                }
            break;

            case '3':
                if (player.custom_map_found == 1) {
                    startgame(maze, playerstart, key, key_color, &player, 1);
                }
            else {
                printf("No custom map found.\n");
            }
            break;

            case '4':
                custominstructions(key_color);
            break;

            case '5':
                printf("\n\n\t\t\t\t\t Name: \033[32m%s\n\033[0m", player.name);
            printf("\t\t\t\t\t Levels beaten: \033[35m%d/%d\n\033[0m", player.level - 1, MAXLEVEL);
            printf("\t\t\t\t\t Deaths: \033[31m%d\n\033[0m", player.deaths);
            printf("\t\t\t\t\t Total Moves Taken: \033[34m%d\n\n\n\033[0m", player.moves);
            break;

            case '6':
            printf("\n\n");
            printf("\t\t\t\t\t     ------------------------------------------------\n\n");
            printf("\t\t\t\t\t     Aaron Beard: Design Lead\n\n");
            printf("\t\t\t\t\t     Chris Justin Taer: Visual Design Engineer\n\n");
            printf("\t\t\t\t\t     Rens Layco Moreno: Project Manager\n\n");
            printf("\t\t\t\t\t     Cloud Daet: Creative Design Director\n\n");
            printf("\t\t\t\t\t     John Andrew Lonceras: Quality Control Specialist\n\n");
            printf("\t\t\t\t\t     ------------------------------------------------\n\n");
            break;

            case '7':
                printf("\033[32mThank you for playing!\n\033[0m");
            break;

            default:
                printf("\n\033[31mInvalid choice\n\033[0m");
            break;
        }
    }

    return 0;
}

void display(int maze[MAXROWS][MAXCOLS], char key[], int key_color[], struct data player) {
    //this function will display the current configuration to the player
    printf("\n");
    for (int i = 0; i < MAXROWS; i++) {
        if (maze[i][0] == 0) {
            printf("\n");
            printf("\n\033[33mP\033[0m: Player, \033[3%dm%c\033[0m: Wall, \033[3%dm%c\033[0m: Goal, \033[3%dm%c\033[0m: Key, \033[3%dm%c\033[0m: Door, \033[3%dm%c\033[0m: Pitfall, \033[3%dm%c\033[0m: Ice, \033[3%dm%c\033[0m: Teleporter\n", key_color[1], key[1], key_color[2], key[2], key_color[3], key[3], key_color[4], key[4], key_color[5], key[5], key_color[6], key[6], key_color[7], key[7]);
            return;
        }
        for (int j = 0; j < MAXCOLS; j++) {
            if (i == player.row && j == player.col) {
                printf("\033[33mP\033[0m");
            }
            else {
                printf("\033[3%dm%c\033[0m", key_color[maze[i][j]], key[maze[i][j]]);
            }
        }
        printf("\n");
    }
    printf("\n\033[33mP\033[0m: Player, \033[3%dm%c\033[0m: Wall, \033[3%dm%c\033[0m: Goal, \033[3%dm%c\033[0m: Key, \033[3%dm%c\033[0m: Door, \033[3%dm%c\033[0m: Pitfall, \033[3%dm%c\033[0m: Ice, \033[3%dm%c\033[0m: Teleporter\n", key_color[1], key[1], key_color[2], key[2], key_color[3], key[3], key_color[4], key[4], key_color[5], key[5], key_color[6], key[6], key_color[7], key[7]);
}

int initialize(int maze[MAXLEVEL][MAXROWS][MAXCOLS], int playerstart[MAXLEVEL][2], struct data *player) {
    //this function should read the maze.txt file to get the data for all the levels in the game, and save that data in the appropriate arrays

    int rows, cols, i, j;
    FILE* ifp = fopen("maze.txt", "r");
    if (ifp == NULL) {
        printf("\033[31mError: Could not open map file.\n\033[0m");
        return -1;
    }
    for (int layer = 1; layer <= MAXLEVEL; layer++) {
        if (fscanf(ifp, "%d %d %d %d", &rows, &cols, &playerstart[layer][0], &playerstart[layer][1]) != 4) {
            printf("\033[31mError: Corrupted Map File\n\033[0m");
            fclose(ifp);
            return -1;
        }
        if (rows > MAXROWS - 2 || cols > MAXCOLS - 2) {
            printf("\033[31mError: Corrupted Map File\n\033[0m");
            fclose(ifp);
            return -1;
        }
        if (playerstart[layer][0] < 1 || playerstart[layer][0] > rows || playerstart[layer][1] < 1 || playerstart[layer][1] > cols) {
            printf("\033[31mError: Corrupted Map File\n\033[0m");
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
    FILE *custom_map = fopen("custom.txt", "r");
    if (custom_map != NULL) {
        if (fscanf(custom_map, "%d %d %d %d", &rows, &cols, &playerstart[MAXLEVEL + 1][0], &playerstart[MAXLEVEL + 1][1]) != 4) {
            printf("\033[31mError: Improper Custom File Layout\n\033[0m");
            fclose(ifp);
            return -1;
        }
        if (rows > MAXROWS - 2 || cols > MAXCOLS - 2) {
            printf("\033[31mError: Invalid Custom Map Size\n\033[0m");
            fclose(ifp);
            return -1;
        }
        if (playerstart[MAXLEVEL + 1][0] < 1 || playerstart[MAXLEVEL + 1][0] > rows || playerstart[MAXLEVEL + 1][1] < 1 || playerstart[MAXLEVEL + 1][1] > cols) {
            printf("\033[31mError: Invalid Custom Map Starting Location\n\033[0m");
            fclose(ifp);
            return -1;
        }
        for (j = 0; j < cols + 2; j++) {
            maze[MAXLEVEL + 1][0][j] = 1;
        }
        for (i = 1; i <= rows; i++) {
            maze[MAXLEVEL + 1][i][0] = 1;
            for (j = 1; j <= cols; j++) {
                if (fscanf(custom_map, "%d", &maze[MAXLEVEL + 1][i][j]) != 1) {
                    printf("\033[31mError: Invalid Custom Map Data\n\033[0m");
                }
            }
            maze[MAXLEVEL + 1][i][j] = 1;
        }
        for (j = 0; j < cols + 2; j++) {
            maze[MAXLEVEL + 1][i][j] = 1;
        }
    }
    player->custom_map_found = 1;
    fclose(custom_map);
    return 0;
}

int getnewplayerdata(struct data *player) {
    //this function will get the new player data from the user
    printf("\033[32mEnter your name: \033[0m");
    scanf("%s", player->name);
    player->level = 1;
    player->deaths = 0;
    player->moves = 0;
    savedata(*player);
    return 0;
}

int getsavedata(struct data *player) {
    //this function will get the saved data from a returning player
    FILE* ifp = fopen("save.txt", "r");
    if (ifp == NULL) {
        return -1;
    }
    fscanf(ifp, "%s", &player->name);
    fscanf(ifp, "%d", &player->level);
    fscanf(ifp, "%d", &player->deaths);
    fscanf(ifp, "%d", &player->moves);
    fclose(ifp);
    return 0;
}

int savedata(struct data player) {
    //this function will save the players current data to their save file
    FILE* ofp = fopen("save.txt", "w");
    if (ofp == NULL) {
        printf("Error: Could not open save file.\n");
        return -1;
    }
    fprintf(ofp, "%s ", player.name);
    fprintf(ofp, "%d ", player.level);
    fprintf(ofp, "%d ", player.deaths);
    fprintf(ofp, "%d ", player.moves);
    fclose(ofp);
    return 0;
}

void startgame(int maze[MAXLEVEL][MAXROWS][MAXCOLS], int playerstart[MAXLEVEL][2], char key[], int key_color[], struct data *player, int custom) {
    //this function is the function that will manage the player between levels and load appropriate levels

    int levelchosen = 0;
    char levelinput;
    char playing = '1';
    while (playing == '1') {
        if (custom) {
            levelchosen = MAXLEVEL + 1;
        }

        else if (player->level > MAXLEVEL) {
            printf("\n\n\t\t\t\t\t\033[32mYou have already beat the game!\n\n\033[0m");
            printf("If you want to choose a level to play, enter \033[32m1\033[0m. If you wish to go back to the main menu, enter \033[31m0\033[0m: ");
            scanf(" %c", &playing);
            if (playing == '1') {
                printf("Enter the level you wish to play: ");
                scanf(" %c", &levelinput);
                levelchosen = levelinput - 48;
                while (levelchosen < 1 || levelchosen > MAXLEVEL) {
                    printf("\033[31mInvalid Choice\n\033[0m");
                    printf("Enter the level you wish to play: ");
                    scanf(" %c", &levelinput);
                    levelchosen = levelinput - 48;
                }
            }
        }

        else {
            levelchosen = player->level;
        }
        if (playing == '1') {
            preparelevel(levelchosen, maze, playerstart, player);
            playlevel(maze[0], key, key_color, player, custom, levelchosen);
            savedata(*player);
        }
        if (custom == 0 && playing == '1') {
            printf("\n\nWould you like to keep playing?\nEnter \033[32m1\033[0m for yes, or \033[31m0\033[0m for no: ");
            scanf(" %c", &playing);
        }
        else {
            playing = '0';
        }
    }
}

void preparelevel(int level, int maze[MAXLEVEL][MAXROWS][MAXCOLS], int playerstart[MAXLEVEL][2], struct data *player) {
    //this function will load the proper level data into the active level slot, layer 0  of the maze array

    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            maze[0][i][j] = maze[level][i][j];
        }
    }
    player->row = playerstart[level][0];
    player->col = playerstart[level][1];
    player->keys = 0;
}

void playlevel(int maze[MAXROWS][MAXCOLS], char key[], int key_color[], struct data *player, int custom, int levelplayed) {
    //this function will run through the turn by turn actions of the level itself, ie display screen, get player move, determine what happens, update player
    int playing_level = 0;
    while (!playing_level) {
        display(maze, key, key_color, *player);
        int move = getmove();
        if (custom == 0) {
            player->moves++;
        }
        playing_level = gamelogic(maze, move, player);
    }
    switch (playing_level) {
        case 1:
            display(maze, key, key_color, *player);
        printf("\n\t\t\t\t\t    \033[33m You Won! \n");
        printf("\n---------------|Player: %s |-------------|Level: %d |---------------|Deaths: %d |-----------------\033[0m\n\n", player->name, levelplayed, player->deaths);
        if (custom == 0) {
            if (player->level < MAXLEVEL + 1) {
                player->level++;
            }
        }
        return;

        case -1:
            if (custom == 0) {
                player->deaths++;
            }
            printf("\n\t\t\t\t\t    \033[31m You Lost! \n");
            printf("\n---------------|Player: %s |-------------|Level: %d |---------------|Deaths: %d |-----------------\033[0m\n\n", player->name, levelplayed, player->deaths);
        return;
    }
}

int getmove(void) {
    char move;
    printf("\nEnter W, A, S, or D to choose your direction, or F to forfeit the level, then press enter: ");
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

        default:
        return 0;
    }
}

int gamelogic(int maze[MAXROWS][MAXCOLS], int move, struct data *player) {

    if (move == 0) {
        return 0;
    }

    //if the player chose to forfeit
    if (move == 5) {
        return -1;
    }

    //these lines look super funky, but basically if they choose 2 or 4, they will move left or right, 1 or 3 will move up or down
    if (move % 2) {
        //we can subtract 3 from 2 or 4 to get -1 or +1 and add that to the appropriate player coordinates to find the target tile
        player->targetrow = player->row + (move - 2);
        player->targetcol = player->col;
    }
    else {
        //we can subtract 2 from 1 or 3 to get -1 or +1 and add that to the appropriate player coordinates to find the target tile
        player->targetrow = player->row;
        player->targetcol = player->col + (move - 3);
    }

    //target type will be the tile type the player is trying to move onto
    int targettype = maze[player->targetrow][player->targetcol];

    switch (targettype) {
        case 0:
        player->row = player->targetrow;
        player->col = player->targetcol;
        return 0;

        case 1:
        return 0;

        case 2:
        player->row = player->targetrow;
        player->col = player->targetcol;
        return 1;

        case 3:
        player->keys++;
        player->row = player->targetrow;
        player->col = player->targetcol;
        maze[player->row][player->col] = 0;
        return 0;

        case 4:
        if (player->keys > 0) {
            player->row = player->targetrow;
            player->col = player->targetcol;
            player->keys--;
            maze[player->targetrow][player->targetcol] = 0;
        }
        return 0;

        case 5:
        return -1;

        case 6:
        player->row = player->targetrow;
        player->col = player->targetcol;
        return gamelogic(maze, move, player);

        case 7:
            for (int i = 0; i < MAXROWS; i++) {
                for (int j = 0; j < MAXCOLS; j++) {
                    if (maze[i][j] == 7 && (i != player->targetrow || j != player->targetcol)) {
                        player->row = i;
                        player->col = j;
                    }
                }
            }
        return 0;

        default:
        return -1;
    }
}

void custominstructions(int key_color[]) {
    printf("\nYour custom map should be saved as 'custom.txt'\n\n");
    printf("The game will import your map automatically\n\n");
    printf("To access your map, try to restart your game\n\n");
    printf("The text should look like this rows(Max:%d) columns(Max:%d)\n\n", MAXROWS - 2, MAXCOLS - 2);
    printf("Next put in the player's starting location within your own maze\n\n");
    printf("Using your own rows and columns choose a space for it. 1 is the leftmost row and topmost column \n\n");
    printf("NOTE: THE PLAYER'S STARTING POSITION MUST BE AN OPEN SPACE\n\n");
    printf("Your first line should look like this. (20 20 1 1)\n\n");
    printf("The game will automatically surround your map with walls. Go free with your imagination!\n\n");
    printf("Using the following keys in the file will get you these tiles:\n\n");
    printf("0-Empty Space, \033[3%dm1-Wall\033[0m, \033[3%dm2-Goal\033[0m, \033[3%dm3-Key\033[0m, \033[3%dm4-Door\033[0m, \033[3%dm5-Pitfall\033[0m, \033[3%dm6-Ice\033[0m, \033[3%dm7-Teleporter\033[0m\n\n", key_color[1], key_color[2], key_color[3], key_color[4], key_color[5], key_color[6], key_color[7]);
    printf("Separate all your tiles with a space\n\n");
    printf("TIP: Arrange your map into a square or rectangle\n");
    printf("\nDo you wish to have an explanation of how each space works?\n");
    printf("Enter 1 for yes, enter 0 for no: ");
    char choice;
    scanf(" %c", &choice);
    if (choice != '0') {
        printf("\nEmpty space(0): The player can freely move into this space from any orthogonal adjacent space\n");
        printf("\033[3%dmWall(1): This space cannot be entered by the player\n\033[0m", key_color[1]);
        printf("\033[3%dmGoal(2): This is the space the player must move to in order to win the level\n\033[0m", key_color[2]);
        printf("\033[3%dmKey(3): If the player enters this space, they can pick up a key that can be consumed to open a door\n\033[0m", key_color[3]);
        printf("\033[3%dmDoor(4): This space represents a locked door that cannot be opened without consuiming a key\n\033[0m", key_color[4]);
        printf("\033[3%dmPitfall(5): This space represents a trap that will cause the player to lose if they move into it\n\033[0m", key_color[5]);
        printf("\033[3%dmIce(6): If the player enters an ice space, they will continue to slide in the same direction until they end up entering another type of space\n\033[0m", key_color[6]);
        printf("\033[3%dmTeleporter(7): If the player enters a teleporter, the player will be moved to the other teleporter space\n\033[0mPlease note that all maps must have exactly 0 or 2 teleporters, or they will not work properly\n", key_color[7]);
    }
    printf("\nWould you like to see an example of how to format your text file for your custom map?\n");
    printf("Enter 1 for yes, enter 0 for no: ");
    scanf(" %c", &choice);
    if (choice != '0') {
        printf("\n5 5 1 2\n");
        printf("1 0 3 4 1\n");
        printf("1 1 1 0 1\n");
        printf("0 0 0 0 1\n");
        printf("0 1 1 1 1\n");
        printf("0 0 0 0 2\n");
        printf("\nIn the above example, the player would start in the empty space next to the key, and then be able to open the door and proceed to the goal.\n");
    }
}