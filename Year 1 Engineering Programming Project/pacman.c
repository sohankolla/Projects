#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "colours.cpp" // imports colours

// For code organization + readability
#define PACMAN 'P'
#define GHOST 'G'
#define DOT '.'
#define WALL 'W'
#define EMPTY ' '
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'

char** loadMap(){ // this function loads the map from map.txt
    FILE* mapPointer;
    mapPointer = fopen("map.txt", "r");

    // creating 2d-pointer array
    char **mapData = (char **) malloc(11 * sizeof(char*)); // allocates memory for a 11x11 pointer array
    for (int i = 0; i < 11; i++){
        mapData[i] = (char*) malloc(11 * sizeof(char)); // makes the array above of 11 into a 2d array of 11x11
        mapData[i][0] = WALL; // makes the left wall on the outside of the map
        mapData[i][10] = WALL; // makes the right wall on the outside of the map
    }

    for (int j = 1; j < 10; j++) {
        mapData[0][j] = WALL; // makes the top wall on the outside of the map
        mapData[10][j] = WALL; // makes the bottom wall on the outside of the map
        for (int k = 1; k < 10; k++) { // goes throguh all 9 lines in the map.txt
            fscanf(mapPointer, "%c ", &mapData[j][k]);
            // scans the map and stores the walls, ghosts, dots, and pacman
            // into the 11x11 pointer array from before.
        }
    }
    fclose(mapPointer); // closes the file
    return mapData;
}

void printMap(char** map){
    for (int i = 0; i < 11; i++){
        for (int j = 0; j < 11; j++) {
            if (j == 10){
                colourChange(BLUE); // changes the right wall to blue
                printf("%c\n", map[i][j]);
                // prints with a new line for the last character of a row for
                // formatting of the map
            }
            else{
                if (map[i][j] == PACMAN) {
                    colourChange(YELLOW); // changes pacman to yellow
                    printf("%c  ", map[i][j]); // prints with spaces for formatting
                } else if (map[i][j] == GHOST) {
                    colourChange(PINK); // changes ghosts to pink
                    printf("%c  ", map[i][j]); // prints with spaces for formatting
                } else if (map[i][j] == WALL) {
                    colourChange(BLUE); // changes walls to blue
                    printf("%c  ", map[i][j]); // prints with spaces for formatting
                } else {
                    colourChange(WHITE); // makes dots white
                    printf("%c  ", map[i][j]); // prints with spaces for formatting
                }
            }
        }
    }
    return;
}

int winCheck(char** dotMap){
    // this function takes the dot locations as a parameter
    // and returns if all of them have been collected or not
    // i.e. if the player has won or not.
    for (int i = 0; i < 11; i++){
        for (int j = 0; j < 11; j++) {
            if (dotMap[i][j] == 1){
                // if there is a dot anywhere on the dot map, it will return zero
                return 0;
            }
        }
    }
    // if there are no dots left, it will return 1 to signify that the user won
    return 1;
}

int loseCheck(char** map){
    // this function takes the map as a parameter
    // and returns if pacman has been caught by the ghost or not
    // i.e. if the player has lost or not
    for (int i = 0; i < 11; i++){
        for (int j = 0; j < 11; j++) {
            if (map[i][j] == PACMAN){
                // checks if pacman is anywhere on the map
                // if not, it means the ghost has been written
                // over pacman in the same spot which means
                // pacman got caught

                // returns 0 if pacman is still in the map/not caught
                return 0;
            }
        }
    }
    // returns 1 if pacman is not in the map/has been caught
    return 1;
}

char randomDirection(){
    // this function creates a random direction for the ghosts to move in
    char direction;
    // srand(time(NULL)); 
    // seeds the rand() function based on the current time so that
    // it is a pseudo-random number generator

    int randomNum = (rand() % 4) + 1; // generates a "random" number between 1 and 4

    if (randomNum == 1){ // each number from 1 to 4 is binded to a different direction
        direction = UP; // 1 is up
    } else if (randomNum == 2){
        direction = DOWN; // 2 is down
    } else if (randomNum == 3){
        direction = LEFT; // 3 is left
    } else {
        direction = RIGHT; // 4 is right
    }

    return direction;
}

int isWall(char** map, int x, int y){
    // this function takes the map, and a point and returns if there is a wall at that point or not
    if (map[x][y] == WALL){
        // if there is a wall, returns 1
        return 1;
    }

    // if there isn't a wall, returns 0
    return 0;
}

char** dotLocation(char** map){
    // this function takes the map and creates another 2-d array of just the walls and the dots
    char** dotMap = (char**) malloc(11 * sizeof(char*)); // 11x11 pointer array
    for (int i = 0; i < 11; i++){
        dotMap[i] = (char*) malloc(11 * sizeof(char)); // makes it 2d
    }

    for (int j = 0; j < 11; j++){
        for (int k = 0; k < 11; k++) {
            if (map[j][k] != PACMAN && map[j][k] != WALL){
                // if there is no pacman or wall, there will be a dot there
                dotMap[j][k] = 1; // saves all the dot locations as a 1
            }
            else if (map[j][k] != PACMAN && map[j][k] != GHOST){
                // if there is no pacman or ghost, there will be a wall there (excluding the dots from above)
                dotMap[j][k] = 2; // saves all the dot locations as a 2
            }
            else {
                dotMap[j][k] = 0; // saves the spot where pacman starts as a 0
                // also, 0s on this map will signify that there is no dot or wall there, i.e. empty
            }
        }
    }

    return dotMap;
}

char** removeDot(char** map, char** dotMap){
    // this function takes the map and dot map as parameters and removes dots whereever
    // pacman moves
    for (int i = 0; i < 11; i++){
        for (int j = 0; j < 11; j++) {
            if (map[i][j] == PACMAN && dotMap[i][j] == 1){
                // if pacman is at a spot on the game map where there is a dot on the dot map,
                // it will set the dot map to have a 0 or "empty" signifying that there is no dot there
                dotMap[i][j] = 0;
            }
        }
    }

    return dotMap;
}

int* pacmanStart(char** map){
    // this function takes the game map as a parameter
    // and finds pacman's starting location
    int* originalLocation = (int*) malloc(2 * sizeof(int));

    for (int j = 0; j < 11; j++){
        for (int k = 0; k < 11; k++) {
            if (map[j][k] == PACMAN){
                // searches through the map for pacman
                // when found:
                originalLocation[0]= j; // saves the x-coord
                originalLocation[1]= k; // saves the y-coord
                return originalLocation;
            }
        }
    }
}

int* pacmanMove(char** map, int* currentPacman, char input){
    // this function takes the map, pacman's location, and the user input as parameters
    // and moves pacman
    int* newLocation = (int*) malloc(2 * sizeof(int)); // pointer array for pacman's next location

    if (input == UP){ // if input is 'w', moves the pacman up
        newLocation[0] = currentPacman[0] - 1;
        newLocation[1] = currentPacman[1];
    } else if (input == DOWN){ // if input is 's', moves the pacman down
        newLocation[0] = currentPacman[0] + 1;
        newLocation[1] = currentPacman[1];
    }
    else if (input == LEFT){ // if input is 'a', moves the pacman left
        newLocation[0] = currentPacman[0];
        newLocation[1] = currentPacman[1] - 1;
    }
    else if (input == RIGHT){ // if input is 'd', moves the pacman right
        newLocation[0] = currentPacman[0];
        newLocation[1] = currentPacman[1] + 1;
    }

    if (isWall(map, newLocation[0], newLocation[1]) == 1){
        // if there is a wall at the spot pacman is going to move to
        // it just doesn't move pacman and returns the old location
        return currentPacman;
    }

    return newLocation;
}

int ghostCount(char** map){
    // counts the number of ghosts in case different maps have different numbers of ghosts
    int numGhosts = 0;
    for (int j = 0; j < 11; j++){
        for (int k = 0; k < 11; k++) {
            if (map[j][k] == GHOST){
                numGhosts++;
            }
        }
    }

    return numGhosts;
}

int** ghostLocation(char** map, int numGhosts){
    // this function finds the original location of the ghosts

    int** coords = (int **) malloc(2 * sizeof(int*));
    for (int i = 0; i < 2; i++){
        coords[i] = (int*) malloc(numGhosts * sizeof(int));
        // makes a 2d-array that can store the x,y coords for the number of ghosts counted in ghostCount
    }


    int n = 0;
    for (int j = 0; j < 11; j++){
        for (int k = 0; k < 11; k++) {
            if (map[j][k] == GHOST){
                // for finding the ghosts coords by going through the array and storing the values
                coords[n][0] = j;
                coords[n][1] = k;
                n++;
            }
        }
    }

    return coords;
}

int** ghostMovement(char** map, int** currentGhostLocation, int* pacmanLastLocation, int numGhosts){

    // creates a new numGhost x 2 array to store all the next ghost locations after moving
    int** newGhostLocation = (int **) malloc(2 * sizeof(int*));
    for (int i = 0; i < 2; i++){
        newGhostLocation[i] = (int*) malloc(numGhosts * sizeof(int));
    }

    char direction;

    // randomizes each ghost movement, depending on numGhosts
    // also in the randomizer, excludes the direction of the wall
    // creates a new direction if that one moves the ghost into a wall
    for (int j = 0; j < numGhosts; j++) {
        do {
            direction = randomDirection(); // calls the random direction generator from before
            // based on the direction generated, creates a new location for the ghost
            if (direction == UP) {
                newGhostLocation[j][0] = currentGhostLocation[j][0] - 1;
                newGhostLocation[j][1] = currentGhostLocation[j][1];
            } else if (direction == DOWN) {
                newGhostLocation[j][0] = currentGhostLocation[j][0] + 1;
                newGhostLocation[j][1] = currentGhostLocation[j][1];
            } else if (direction == LEFT) {
                newGhostLocation[j][0] = currentGhostLocation[j][0];
                newGhostLocation[j][1] = currentGhostLocation[j][1] - 1;
            } else if (direction == RIGHT) {
                newGhostLocation[j][0] = currentGhostLocation[j][0];
                newGhostLocation[j][1] = currentGhostLocation[j][1] + 1;
            }
            // while loop to check if the new ghost location would write over a wall
            // if it does, it would rerun until the ghost has a valid new location
        } while (isWall(map, newGhostLocation[j][0], newGhostLocation[j][1]) == 1);
    }

    // after running random movement, we have another for loop that
    // checks which ghosts see pacman
    // the ones that see pacman will get their new ghost location overwritten
    // with a location that goes towards pacman
    for (int k = 0; k < numGhosts; k++) {
        int noWall = 1; // initializes a wall check variable
        if (currentGhostLocation[k][0] == pacmanLastLocation[0]) {
            if (currentGhostLocation[k][1] < pacmanLastLocation[1]){ // if pacman is on the right of ghost
                for (int r = currentGhostLocation[k][1]; r < pacmanLastLocation[1] + 1; r++){
                    if (isWall(map, k, r) == 1){ // checks if there is a wall in between pacman and the ghost
                        noWall = 0; // sets no wall to false, meaning there is a wall
                        break;
                    }
                }
                if (noWall == 1){ // if there's no wall, moves the ghost
                    newGhostLocation[k][0] = currentGhostLocation[k][0];
                    newGhostLocation[k][1] = currentGhostLocation[k][1] + 1;
                }
            }
            else if (currentGhostLocation[k][1] > pacmanLastLocation[1]){ // if pacman is on the left of ghost
                for (int r = pacmanLastLocation[1]; r < currentGhostLocation[k][1] + 1; r++){
                    if (isWall(map, k, r) == 1){ // checks if there is a wall in between pacman and the ghost
                        noWall = 0; // sets no wall to false, meaning there is a wall
                        break;
                    }
                }
                if (noWall == 1){ // if there's no wall, moves the ghost
                    newGhostLocation[k][0] = currentGhostLocation[k][0];
                    newGhostLocation[k][1] = currentGhostLocation[k][1] - 1;
                }
            }
        }
        else if (currentGhostLocation[k][1] == pacmanLastLocation[1]){
            if (currentGhostLocation[k][0] < pacmanLastLocation[0]){ // if pacman is above the ghost
                for (int r = currentGhostLocation[k][0]; r < pacmanLastLocation[0] + 1; r++){ // checks if there is a wall in between pacman and the ghost
                    if (isWall(map, k, r) == 1){ // checks if there is a wall in between pacman and the ghost
                        noWall = 0;
                        break;
                    }
                }
                if (noWall == 1){ // if there's no wall, moves the ghost
                    newGhostLocation[k][0] = currentGhostLocation[k][0] + 1;
                    newGhostLocation[k][1] = currentGhostLocation[k][1];
                }
            }
            else if (currentGhostLocation[k][1] > pacmanLastLocation[1]){ // if pacman is below the ghost
                for (int r = pacmanLastLocation[0]; r < currentGhostLocation[k][0] + 1; r++){ // checks if there is a wall in between pacman and the ghost
                    if (isWall(map, k, r) == 1){ // checks if there is a wall in between pacman and the ghost
                        noWall = 0;
                        break;
                    }
                }
                if (noWall == 1){ // if there's no wall, moves the ghost
                    newGhostLocation[k][0] = currentGhostLocation[k][0] - 1;
                    newGhostLocation[k][1] = currentGhostLocation[k][1];
                }
            }
        }
    }

    return newGhostLocation;
}

char** editMap(char** map, char** dotMap, int** ghostLocations, int* pacmanLocation, int numGhosts){

    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            if (dotMap[i][j] == 1){ // based on the dot map, updates the dots
                map[i][j] = DOT; // draws in dots
            }
            else if (dotMap[i][j] == 2){ // based on the dot map, redraws the walls
                map[i][j] = WALL; // draws in walls
            }
            else {
                map[i][j] = EMPTY; // everything else would be empty
            }
        }
    }

    map[pacmanLocation[0]][pacmanLocation[1]] = PACMAN; // draws in pacman

    for (int n = 0; n < numGhosts; n++) {
        map[ghostLocations[n][0]][ghostLocations[n][1]] = GHOST; // inserts the ghosts
    }

    return map;
}


int main() {
    char input; // user input variable

    char **map = loadMap(); // loads the map
    printMap(map); // prints the original map

    char **dotMap = dotLocation(map); // saves the dots and walls

    int* pacmanLocation = pacmanStart(map); // declares and initializes pacman's position array
    int* pacmanLastLocation; // creates a temp variable for pacman location

    int numGhosts = ghostCount(map); // counts the number of ghosts on the map
    int** ghostLocations = ghostLocation(map, numGhosts); // declares and initializes the ghost's position 2d-array

    do {
        input = getch(); // gets user input

        if (input == UP || input == DOWN || input == LEFT || input == RIGHT) { // only runs if the input is wasd
            pacmanLastLocation = pacmanLocation;
            pacmanLocation = pacmanMove(map, pacmanLocation, input); // moves pacman
            dotMap = removeDot(map, dotMap); // updates dot map
            ghostLocations = ghostMovement(map, ghostLocations, pacmanLastLocation, numGhosts); // moves ghosts

            map = editMap(map, dotMap, ghostLocations, pacmanLocation, numGhosts); // updates the map
            printf("\n");
            printMap(map); // prints the map
        }
        // runs the game as long the user doesn't win or lose
        // also quits when the input is 'q' for quick testing
    } while (input != 'q' && winCheck(dotMap) == 0 && loseCheck(map) == 0);

    if (winCheck(dotMap) == 1) {
        colourChange(WHITE);
        printf("Congratulations! You win! Press any key to exit the game");
        getch();
    } else if (loseCheck(map) == 1) {
        colourChange(WHITE);
        printf("Sorry, you lose. Press any key to exit the game");
        getch();
    }

    return 0;
}
