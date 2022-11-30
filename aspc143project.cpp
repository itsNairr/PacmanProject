// APSC 143 Engineering Programming Project Starter Code
// Feel free to change anything about this file, but do not remove "#include "colours.h".

// Make sure to include all relevant libraries
#include <stdio.h>
#include "colours.h"
#include <stdlib.h>

// You don't need to use these, but they help make your code look more organized
#define PACMAN 'P'
#define GHOST 'G'
#define DOT '.'
#define WALL 'W'
#define EMPTY ' '
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'

// Below are the required functions for this program, but you will need to create your own as well.
// Make sure to specify the required parameters, if any.
FILE *openFile(); //declaring all functions
char **readMap(FILE *fmap);//---
void printMap();//---
int pacMove(char move, int loc[]);//---
int gMove(char move, int loc[]);//---
int isWall(int loc[], char move);//---
void masterPac();//---
int winCheck();//---
int loseCheck();//---
void ghostMove(char gmove[], int gtoggle[]);//---

char** map;
int location[2], glocation[2][2]; //locations for both PACMAN and Ghosts
int main() { //Main function
    FILE *fmap = openFile();//Open Map.txt
    map = readMap(fmap);//read map
    masterPac();//Master function to work the game
    return 0;
}

FILE *openFile() {
    FILE *fmap;//opening the map file
    fmap = fopen("map.txt", "r");
    if (fmap == NULL) {
        printf("File not found\n"); //outputs if file not found

    }
    return fmap; //return opened map
}

char **readMap(FILE *fmap) { //readmap function that places the text values into an array
    int j = 0;
    map = (char **) malloc(11 * sizeof(char *));
    for (int i = 0; i < 11; i++) {
        map[i] = (char *) malloc(11 * sizeof(char));
    }
    for (int i = 0; i < 11; i++) {
        for (int k = 0; k < 11; k++) {
            if (i == 0 || i == 10 || k == 0 || k == 10) {
                map[k][i] = WALL;
            } else {
                fscanf(fmap, "%s", &map[k][i]);
                if (map[k][i] == PACMAN) { //pacman is stored elsewhere, so it will be replaced as a empty space
                    map[k][i] = EMPTY;
                    location[0] = k;//x
                    location[1] = i;//y
                } else if (map[k][i] == GHOST) { //ghosts are also stored elsewhere, so they will be replaced as a dot
                    map[k][i] = DOT;
                    glocation[j][0] = k;//x
                    glocation[j][1] = i;//y
                    j++;
                }

            }
        }
    }
    return map;//map array with data is returned
}

void printMap() { //prints the map into the terminal
    for (int i = 0; i < 11; i++) {
        for (int k = 0; k < 11; k++) {
            if (location[0] == k && location[1] == i) { //prints pacman
                colourChange(YELLOW);
                printf("P  ");
            } else if (glocation[0][0] == k && glocation[0][1] == i) { //prints ghost 1
                colourChange(PINK);
                printf("G  ");
            } else if (glocation[1][0] == k && glocation[1][1] == i) {  //prints ghost 2
                colourChange(PINK);
                printf("G  ");
            } else {
                if (map[k][i] == WALL) { //prints walls
                    colourChange(BLUE);
                    printf("%c  ", map[k][i]);
                } else {
                    colourChange(WHITE);//prints dots
                    printf("%c  ", map[k][i]);
                }
            }
        }
        printf("\n");
    }
    return;
}

void ghostMove(char gmove[], int gtoggle[]) { //Ai function for the ghosts
    int num=0, nnum=0;
    int tog[2] = {0,0};//toggles for each ghost
    //---------------------------------------- PACMAN check
    for (int j = 0; j < 2; j++) { //first the function scans if pacman is around each ghost
        for (int i = glocation[j][1]; map[glocation[j][0]][i] != WALL; i++) { //loop stops if it does not see pacman and instead sees a wall
            if (glocation[j][0] == location[0] && i == location[1]) {// looking for pacman below the ghost
                gmove[j] = DOWN;//move down
                gtoggle[j] = 1;//toggle to check x "view" next time it is called
                //printf("\nD\n");
                tog[j] = 1;//becomes 1 when pacman is found
            }
        }
        for (int i = glocation[j][1]; map[glocation[j][0]][i] != WALL; i--) {
            if (glocation[j][0] == location[0] && i == location[1]) {// looking for pacman above the ghost
                gmove[j] = UP;//move up
                gtoggle[j] = 1;//toggle to check x "view" next time it is called
                //printf("\nU\n");
                tog[j] = 1;//becomes 1 when pacman is found
            }
        }
        for (int i = glocation[j][0]; map[i][glocation[j][1]] != WALL; i++) {
            if (glocation[j][1] == location[1] && i == location[0]) {//looking for pacman to the right of the ghost
                gmove[j] = RIGHT;//move right
                gtoggle[j] = 0;//toggle to check y "view" next time it is called
                //printf("\nR\n");
                tog[j] = 1;
            }
        }
        for (int i = glocation[j][0]; map[i][glocation[j][1]] != WALL; i--) {
            if (glocation[j][1] == location[1] && i == location[0]) {//looking for pacman to the left of the ghost
                gmove[j] = LEFT;//move left
                gtoggle[j] = 0;//toggle to check y "view" next time it is called
                //printf("\nL\n");
                tog[j] = 1;//becomes 1 when pacman is found
            }
        }
    }
  //----------------------------------------
        for (int j = 0; j < 2; j++) { //loop for each ghost
            if(tog[j] == 0){ //if pacman was not found above, the tog of whichever ghost will be 0, and thus the below function will execute
            num = 0;//setting comparing variables to 0
            nnum = 0;
            if (isWall(glocation[j], gmove[j]) == 1 && gtoggle[j] == 0) {//if ghost runs into a wall on its respective traveling direction, it will begin to scan
                //printf("wall");
                //---------------------------------------- y check
                for (int y = glocation[j][1]; map[glocation[j][0]][y] != WALL; y++) { //scans the amount of dots and empties above ghost until it finds a wall
                    num ++;
                }
                for (int y = glocation[j][1]; map[glocation[j][0]][y] != WALL; y--) { //scans the amount of dots and empties below ghost until it finds a wall
                    nnum++;
                }
                gtoggle[j] = 1;
                if (num < nnum) { //compares the dots and empties and goes the direction where there is most
                    gmove[j] = UP;//move up
                    //printf("\nGhost %d WU\n",j);
                } else {
                    gmove[j] = DOWN;//move down
                    //printf("\nGhost %d WD\n",j);
                }
            }
                //----------------------------------------
                //---------------------------------------- x check
            else if (isWall(glocation[j], gmove[j]) == 1 && gtoggle[j] == 1) { //if ghost runs into a wall on its respective traveling direction, it will begin to scan
                //printf("\nGhost %d In the matrix\n",j);
                for (int x = glocation[j][0]; map[x][glocation[j][1]] != WALL; x++) { //scans the amount of dots and empties right of ghost until it finds a wall
                    num++; //scanning right
                    //printf("\nDEBUG R %d Ghost %d\n", num,j);
                }
                for (int x = glocation[j][0]; map[x][glocation[j][1]] != WALL; x--) { //scans the amount of dots and empties left of ghost until it finds a wall
                    nnum++; //scanning left
                    //printf("\nDEBUG  L %d Ghost %d\n", nnum,j);
                }
                gtoggle[j] = 0;
                if (num < nnum) { //compares the dots and empties and goes the direction where there is most
                    gmove[j] = LEFT;//move to the left
                    //printf("\nGhost %d WL\n",j);
                } else {
                    gmove[j] = RIGHT;// move to the right
                    //printf("\nGhost %d WR\n",j);
                }
            }
            //----------------------------------------
        }
    }
    return;
}

int gMove(char move, int loc[]){ //function to make each ghost move in their respective direction
    if (move == LEFT) {
        if (!isWall(loc, move)) { //if there is no wall in front of ghost, it will move foreward in their direction
            loc[0] -= 1;
        } else {
            return 0;//if it finds a wall it will return
        }
    } else if (move == DOWN) {
        if (!isWall(loc, move)) { //if there is no wall in front of ghost, it will move foreward in their direction
            loc[1] += 1;
        } else {
            return 0;//if it finds a wall it will return
        }
    } else if (move == RIGHT) {
        if (!isWall(loc, move)) { //if there is no wall in front of ghost, it will move foreward in their direction
            loc[0] += 1;
        } else {
            return 0;//if it finds a wall it will return
        }
    } else if (move == UP) {
        if (!isWall(loc, move)) { //if there is no wall in front of ghost, it will move foreward in their direction
            loc[1] -= 1;
        } else {
            return 0;//if it finds a wall it will return
        }
    }
    return 1;
}

int pacMove(char move, int loc[]) { //function to make pacman move in the chosen direction
    //printf("\nDebug Pacman is at: %d %d\n", location[0], location[1]); DEBUGGING
    if (move == LEFT) {
        if (!isWall(loc, move)) { //Pacman will move left if there is no wall in front of it
            //printf("\nmove a\n") DEBUGGING
            loc[0] -= 1;
            //printf("\nDebug Pacman is now at: %d %d\n", location[0], location[1]); DEBUGGING
        } else {
            return 0;
        }
    } else if (move == DOWN) { //Pacman will move down if there is no wall in front of it
        if (!isWall(loc, move)) {
            loc[1] += 1;
        } else {
            return 0;
        }
    } else if (move == RIGHT) { //Pacman will move right if there is no wall in front of it
        if (!isWall(loc, move)) {
            loc[0] += 1;
        } else {
            return 0;
        }
    } else if (move == UP) { //Pacman will move up if there is no wall in front of it
        if (!isWall(loc, move)) {
            loc[1] -= 1;
        } else {
            return 0;
        }
    }
    map[loc[0]][loc[1]] = EMPTY;//removes the dot as pacman "eats" them
    return 1;//return if pacman can move else it will return 0
}

int isWall(int loc[], char move) { //Function determines if pacman or a ghost hits the wall or not
    if (move == 'a') {
        //printf("\naaaaah\n");
        if (map[loc[0] - 1][loc[1]] == WALL) {
            // printf("\nwall %d %d\n", (location[0]-1),(location[1]));
            return 1;//if it hits a wall at a certain direction, it will return 0, meaning that a wall has been hit
        } else {
            //printf("\nno wall %d %d\n", (location[0]-1),(location[1]));
            return 0;//returns 0 if there is no wall
        }
    } else if (move == 's') {
        if (map[loc[0]][loc[1] + 1] == WALL) {
            return 1;//if it hits a wall at a certain direction, it will return 0, meaning that a wall has been hit
        } else {
            return 0;//returns 0 if there is no wall
        }
    } else if (move == 'd') {
        if (map[loc[0] + 1][loc[1]] == WALL) {
            return 1;//if it hits a wall at a certain direction, it will return 0, meaning that a wall has been hit
        } else {
            return 0;//returns 0 if there is no wall
        }
    } else if (move == 'w') {
        if (map[loc[0]][loc[1] - 1] == WALL) {
            return 1;//if it hits a wall at a certain direction, it will return 0, meaning that a wall has been hit
        } else {
            return 0;//returns 0 if there is no wall
        }
    }
    return 0;
}

void masterPac() { //all the functions combine
    char gmove[2] = {DOWN, UP};//ghosts 0 and 1 start off going down and up respectively
    int gtoggle[2] = {1, 1}; //toggles to determine next change in direction both start at 1
    printMap();
    while (!winCheck() && !loseCheck()) { //loop goes on until pacman either wins or loses
        char movement = 0;
        while (movement != 'w' && movement != 'a' && movement != 's' && movement != 'd') {
            movement = getch();//getch function allows to enter 1 char without pressing enter
        }
        if (!isWall(location, movement)) {
            ghostMove(gmove, gtoggle);
            pacMove(movement, location);
            for (int i = 0; i < 2; i++) {
                ghostMove(gmove, gtoggle);
                gMove(gmove[i], glocation[i]);//loop for each ghost to move
                ghostMove(gmove, gtoggle);
            }
            printf("\n");
            printMap();
        }
    }
    if (winCheck()) { //win conditional statement
        colourChange(WHITE);
        printf("\nCongratulations! You win! Press any key to exit the game");
        getch();
        free(map);//free the dynamic memory
        return;//program ends
    }
    if (loseCheck()) { //lose conditional statement
        colourChange(WHITE);
        printf("\nSorry, you lose. Press any key to exit the game");
        getch();
        free(map);//free the dynamic memory
        return;//program ends
    }
}

int winCheck() { //fucntion to check if pacman won
    for (int i = 1; i < 10; i++) { //if the map has dots, the function will return false
        for (int k = 1; k < 10; k++) {
            if (map[k][i] == DOT) {
                return 0;
            }
        }
    }
    return 1;//if the map has no dots left, the function will return true
}

int loseCheck() { //fucntion to check if pacman lost
    for (int i = 0; i < 2; i++) {
        if (location[0] == glocation[i][0] && location[1] == glocation[i][1]) { //if one of the ghosts location is the same as pacmans location, the function will return true
            return 1; //that means that pacman has lost
        }
    }
    return 0;//returns false if pacman has not lost yet and is still in the game
}