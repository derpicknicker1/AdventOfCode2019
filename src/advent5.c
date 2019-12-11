/*

*/

#include "advent.h"

static int *input = NULL;
static int cnt;


/*##########################
# Get input data from file #
##########################*/
static int getInput(char *f) {
    char *line = NULL;
    size_t l = 0;
    cnt = 0;

    FILE *file = fopen(f, "r");
    if (file == NULL) {
        printf("ERR: CAN NOT OPEN '%s'\n\n", f);
        return 0;
    }

    if (getline(&line, &l, file) != -1) {
        char *ptr = strtok(line, ",");

        while (ptr != NULL) {
            input = realloc(input, ++cnt * sizeof(int));
            input[cnt - 1] = toI(ptr);
            ptr = strtok(NULL, ",");
        }
    }
    else
        return 0;

    free(line);
    fclose(file);
    return 1;
}


/*##########################
# Function to solve part A #
##########################*/
void get5a(char * f) {

    if (!getInput(f))
        return;

    int step = 0;
    int output;

    for (int i = 0; i < cnt; i += step) {
        
        int B = input[i] % 10000 / 1000;
        int A = input[i] % 1000 / 100;
        int op = input[i] % 100;
        int in1, in2;

        if ((op == 1 || op == 2) && A == 0)
            in1 = input[input[i + 1]];
        else
            in1 = input[i + 1];
        if ((op == 1 || op == 2) && B == 0)
            in2 = input[input[i + 2]];
        else
            in2 = input[i + 2];

        if (op == 1)
            input[input[i + 3]] = in1 + in2;
        if (op == 2)
            input[input[i + 3]] = in1 * in2;
        if (op == 3)
            input[input[i + 1]] = 1;
        if (op == 4)
            output = input[input[i + 1]];
        if (op == 99)
            break;

        step = (op > 2) ? 2 : 4;
    }


    printf("5a: %d\n", output);

    free(input);
    input = NULL;
}


/*##########################
# Function to solve part B #
##########################*/
void get5b(char *f) {

    if (!getInput(f))
        return;

    printf("5b: %d\n\n", 0);

    free(input);
    input = NULL;
}


