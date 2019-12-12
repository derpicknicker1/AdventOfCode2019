/*
 --- Day 3: Crossed Wires ---
 The gravity assist was successful, and you're well on your way to the Venus refuelling station. During the rush back on Earth, the fuel management system wasn't completely installed, so that's next on the priority list.

 Opening the front panel reveals a jumble of wires. Specifically, two wires are connected to a central port and extend outward on a grid. You trace the path each wire takes as it leaves the central port, one wire per line of text (your puzzle input).

 The wires twist and turn, but the two wires occasionally cross paths. To fix the circuit, you need to find the intersection point closest to the central port. Because the wires are on a grid, use the Manhattan distance for this measurement. While the wires do technically cross right at the central port where they both start, this point does not count, nor does a wire count as crossing with itself.

 For example, if the first wire's path is R8,U5,L5,D3, then starting from the central port (o), it goes right 8, up 5, left 5, and finally down 3:

 ...........
 ...........
 ...........
 ....+----+.
 ....|....|.
 ....|....|.
 ....|....|.
 .........|.
 .o-------+.
 ...........
 Then, if the second wire's path is U7,R6,D4,L4, it goes up 7, right 6, down 4, and left 4:

 ...........
 .+-----+...
 .|.....|...
 .|..+--X-+.
 .|..|..|.|.
 .|.-X--+.|.
 .|..|....|.
 .|.......|.
 .o-------+.
 ...........
 These wires cross at two locations (marked X), but the lower-left one is closer to the central port: its distance is 3 + 3 = 6.

 Here are a few more examples:

 R75,D30,R83,U83,L12,D49,R71,U7,L72
 U62,R66,U55,R34,D71,R55,D58,R83 = distance 159
 R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51
 U98,R91,D20,R16,D67,R40,U7,R15,U6,R7 = distance 135
 What is the Manhattan distance from the central port to the closest intersection?

 Your puzzle answer was 209.

 --- Part Two ---
 It turns out that this circuit is very timing-sensitive; you actually need to minimize the signal delay.

 To do this, calculate the number of steps each wire takes to reach each intersection; choose the intersection where the sum of both wires' steps is lowest. If a wire visits a position on the grid multiple times, use the steps value from the first time it visits that position when calculating the total value of a specific intersection.

 The number of steps a wire takes is the total number of grid squares the wire has entered to get to that location, including the intersection being considered. Again consider the example from above:

 ...........
 .+-----+...
 .|.....|...
 .|..+--X-+.
 .|..|..|.|.
 .|.-X--+.|.
 .|..|....|.
 .|.......|.
 .o-------+.
 ...........
 In the above example, the intersection closest to the central port is reached after 8+5+5+2 = 20 steps by the first wire and 7+6+4+3 = 20 steps by the second wire for a total of 20+20 = 40 steps.

 However, the top-right intersection is better: the first wire takes only 8+5+2 = 15 and the second wire takes only 7+6+2 = 15, a total of 15+15 = 30 steps.

 Here are the best steps for the extra examples from above:

 R75,D30,R83,U83,L12,D49,R71,U7,L72
 U62,R66,U55,R34,D71,R55,D58,R83 = 610 steps
 R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51
 U98,R91,D20,R16,D67,R40,U7,R15,U6,R7 = 410 steps
 What is the fewest combined steps the wires must take to reach an intersection?

 Your puzzle answer was 43258.
*/

#include "advent.h"

#define MAP_X 15000
#define MAP_Y 70000
#define MAP_OFFSET_X MAP_X/2
#define MAP_OFFSET_Y MAP_Y/2

static uint16_t map[MAP_Y][MAP_X] = { };
static int **wire1;
static int **wire2;
static int cntW1 = 0;
static int cntW2 = 0;
static int distStore = 10000000;


int checkValue(int y, int x, int wire);
void calcDist(int y, int x);
void calcSteps(int y, int x, int steps);

enum {
    W1 = -3,
    W2 = -7,
    START = -5,
    R = 2,
    L = 4,
    U = 6,
    D = 8
};

static void cleanup() {

    for (int y = 0; y < MAP_Y; y++)
        for (int x = 0; x < MAP_X; x++)
            map[y][x] = 0;

    for (int i = 0; i < cntW1; i++)
        free(wire1[i]);
    free(wire1);
    wire1 = NULL;

    for (int i = 0; i < cntW2; i++)
        free(wire2[i]);
    free(wire2);
    wire2 = NULL;

    cntW1 = 0;
    cntW2 = 0;
    distStore = 10000000;
}

/*##########################
# Get input data from file #
##########################*/
static int getInput(char *f) {
    char *line = NULL;
    size_t l = 0;
    cntW1 = 0;
    cntW2 = 0;

    FILE *file = fopen(f, "r");
    if (file == NULL) {
        printf("ERR: CAN NOT OPEN '%s'\n\n", f);
        return 0;
    }

    if (getline(&line, &l, file) != -1) {

        char *ptr = strtok(line, ",");

        while (ptr != NULL) {
            char c;
            int length;
            sscanf(ptr, "%c%d", &c, &length);
            int dir = 0;
            switch (c) {
                case 'U': dir = U; break;
                case 'D': dir = D; break;
                case 'L': dir = L; break;
                case 'R': dir = R; break;
            }
            wire1 = realloc(wire1, ++cntW1 * sizeof(int*));
            wire1[cntW1 - 1] = malloc(2 * sizeof(int));
            wire1[cntW1 - 1][0] = dir;
            wire1[cntW1 - 1][1] = length;

            ptr = strtok(NULL, ",");
        }
    }
    else {
        free(line);
        fclose(file);
        return 0;
    }


    if (getline(&line, &l, file) != -1) {

        char *ptr = strtok(line, ",");

        while (ptr != NULL) {
            char c;
            int length;
            sscanf(ptr, "%c%d", &c, &length);
            int dir = 0;
            switch (c) {
                case 'U': dir = U; break;
                case 'D': dir = D; break;
                case 'L': dir = L; break;
                case 'R': dir = R; break;
            }
            wire2 = realloc(wire2, ++cntW2 * sizeof(int*));
            wire2[cntW2 - 1] = malloc(2 * sizeof(int));
            wire2[cntW2 - 1][0] = dir;
            wire2[cntW2 - 1][1] = length;

            ptr = strtok(NULL, ",");
        }
    }
    else {
        free(line);
        fclose(file);
        return 0;
    }

    free(line);
    fclose(file);
    return 1;
}


/*##########################
# Function to solve part A #
##########################*/
void get3a(char * f) {

    if (!getInput(f))
        return;

    int x = MAP_OFFSET_X;
    int y = MAP_OFFSET_Y;
    map[y][x] = START;

    for (int i = 0; i < cntW1; i++) {
        if (y < 0 || x < 0 || x >= MAP_X || y >= MAP_Y) {
            printf("\n\nERR x %d | y %d\n\n", x, y);
            break;
        }

        switch (wire1[i][0]) {
            case U:
                for (int j = 1; j <= wire1[i][1]; j++)
                    if (checkValue(y - j, x, W1))
                        map[y - j][x] = W1;
                y -= wire1[i][1];
                break;
            case D:
                for (int j = 1; j <= wire1[i][1]; j++)
                    if (checkValue(y + j, x, W1))
                        map[y + j][x] = W1;
                y += wire1[i][1];
                break;

            case L:
                for (int j = 1; j <= wire1[i][1]; j++)
                    if (checkValue(y, x - j, W1))
                        map[y][x - j] = W1;
                x -= wire1[i][1];
                break;

            case R:
                for (int j = 1; j <= wire1[i][1]; j++)
                    if (checkValue(y, x + j, W1))
                        map[y][x + j] = W1;
                x += wire1[i][1];
                break;

            default:
                printf("Err dir w1\n");
                break;
        }
    }

    x = MAP_OFFSET_X;
    y = MAP_OFFSET_Y;

    for (int i = 0; i < cntW2; i++) {
        if (y < 0 || x < 0 || x >= MAP_X || y >= MAP_Y) {
            printf("\n\nERR x %d | y %d\n\n", x, y);
            break;
        }

        switch (wire2[i][0]) {
            case U:
                for (int j = 1; j <= wire2[i][1]; j++)
                    if (checkValue(y - j, x, W2))
                        calcDist(y - j, x);
                y -= wire2[i][1];
                break;
            case D:
                for (int j = 1; j <= wire2[i][1]; j++)
                    if (checkValue(y + j, x, W2))
                        calcDist(y + j, x);
                y += wire2[i][1];
                break;
            case L:
                for (int j = 1; j <= wire2[i][1]; j++)
                    if (checkValue(y, x - j, W2))
                        calcDist(y, x - j);
                x -= wire2[i][1];
                break;
            case R:
                for (int j = 1; j <= wire2[i][1]; j++)
                    if (checkValue(y, x + j, W2))
                        calcDist(y, x + j);
                x += wire2[i][1];
                break;
            default:
                printf("Err dir w2\n");
                break;
        }
    }

    printf("3a: %d\n", distStore);

    cleanup();
}


/*##########################
# Function to solve part B #
##########################*/
void get3b(char *f) {

	if(!getInput(f))
		return;

    int x = MAP_OFFSET_X;
    int y = MAP_OFFSET_Y;
    map[y][x] = START;
    int steps = 0;

    for (int i = 0; i < cntW1; i++) {
        if (y < 0 || x < 0 || x >= MAP_X || y >= MAP_Y) {
            printf("\n\nERR x %d | y %d\n\n", x, y);
            break;
        }

        switch (wire1[i][0]) {
            case U:
                for (int j = 1; j <= wire1[i][1]; j++)
                    if (checkValue(y - j, x, W1))
                        map[y - j][x] = steps + j;
                y -= wire1[i][1];
                break;
            case D:
                for (int j = 1; j <= wire1[i][1]; j++)
                    if (checkValue(y + j, x, W1))
                        map[y + j][x] = steps + j;
                y += wire1[i][1];
                break;

            case L:
                for (int j = 1; j <= wire1[i][1]; j++)
                    if (checkValue(y, x - j, W1))
                        map[y][x - j] = steps + j;
                x -= wire1[i][1];
                break;

            case R:
                for (int j = 1; j <= wire1[i][1]; j++)
                    if (checkValue(y, x + j, W1))
                        map[y][x + j] = steps + j;
                x += wire1[i][1];
                break;

            default:
                printf("Err dir w1\n");
                break;
        }
        steps += wire1[i][1];
    }

    x = MAP_OFFSET_X;
    y = MAP_OFFSET_Y;
    steps = 0;

    for (int i = 0; i < cntW2; i++) {
        if (y < 0 || x < 0 || x >= MAP_X || y >= MAP_Y) {
            printf("\n\nERR x %d | y %d\n\n", x, y);
            break;
        }

        switch (wire2[i][0]) {
            case U:
                for (int j = 1; j <= wire2[i][1]; j++)
                    if (checkValue(y - j, x, W2))
                        calcSteps(y - j, x, steps + j);
                y -= wire2[i][1];
                break;
            case D:
                for (int j = 1; j <= wire2[i][1]; j++)
                    if (checkValue(y + j, x, W2))
                        calcSteps(y + j, x, steps + j);
                y += wire2[i][1];
                break;
            case L:
                for (int j = 1; j <= wire2[i][1]; j++)
                    if (checkValue(y, x - j, W2))
                        calcSteps(y, x - j, steps + j);
                x -= wire2[i][1];
                break;
            case R:
                for (int j = 1; j <= wire2[i][1]; j++)
                    if (checkValue(y, x + j, W2))
                        calcSteps(y, x + j, steps + j);
                x += wire2[i][1];
                break;
            default:
                printf("Err dir w2\n");
                break;
        }
        steps += wire2[i][1];
    }

    printf("3b: %d\n\n", distStore);

    cleanup();
}

int checkValue(int y, int x, int wire) {

    if (y < 0 || x < 0 || x >= MAP_X || y >= MAP_Y) {
        printf("ERR x %d | y %d\n", x, y);
        return 0;
    }

    if (map[y][x] == START)
        return 0;

    if (wire == W1 || map[y][x] == W1 || (wire == W2 && map[y][x] > 0))
        return 1;
    else
        return 0;
}

void calcDist(int y, int x) {
    int dist = abs(MAP_OFFSET_X - x) + abs(MAP_OFFSET_Y - y);
    if (dist < distStore)
        distStore = dist;
}

void calcSteps(int y, int x, int steps) {
    int dist = steps + map[y][x];
    if (dist < distStore)
        distStore = dist;
}
