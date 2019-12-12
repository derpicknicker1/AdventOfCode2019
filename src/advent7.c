/*

*/

#include "advent.h"

static int *input = NULL;
static int cnt;

typedef struct {
    int ip = 0;
    int in = 0;
    int out = 0;
    int halt = 0;
    int *work = NULL;
} Computer;

int compute(int param1, int param2);

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
void get7a(char * f) {

    if (!getInput(f))
        return;

    int thrust = 0;

    for(int a = 0; a < 5; a++)
        for(int b = 0; b < 5; b++)
            if(b != a)
                for(int c = 0; c < 5; c++)
                    if(c != b && c != a)
                        for(int d = 0; d < 5; d++)
                            if(d != c && d != b && d != a)
                                for(int e = 0; e < 5; e++)
                                    if(e != d && e != c && e != b && e != a ) {
                                        int tmpThrust = compute(e,compute(d,compute(c,compute(b,compute(a,0)))));
                                        if (tmpThrust > thrust)
                                            thrust = tmpThrust;
                                    }

    printf("7a: %d\n", thrust);

    free(input);
    input = NULL;
}


/*##########################
# Function to solve part B #
##########################*/
void get7b(char *f) {

    if (!getInput(f))
        return;

    int thrust = 0;

    for(int a = 5; a < 10; a++)
        for(int b = 5; b < 10; b++)
            if(b != a)
                for(int c = 5; c < 10; c++)
                    if(c != b && c != a)
                        for(int d = 5; d < 10; d++)
                            if(d != c && d != b && d != a)
                                for(int e = 5; e < 10; e++)
                                    if(e != d && e != c && e != b && e != a ) {
                                        Computer cA, cB, cC, cD, cE;
                                        cA.work = malloc(cnt * sizeof(int));
                                        memcpy(cA.work, input, cnt * sizeof(int));
                                        cB.work = malloc(cnt * sizeof(int));
                                        memcpy(cB.work, input, cnt * sizeof(int));
                                        cC.work = malloc(cnt * sizeof(int));
                                        memcpy(cC.work, input, cnt * sizeof(int));
                                        cD.work = malloc(cnt * sizeof(int));
                                        memcpy(cD.work, input, cnt * sizeof(int));
                                        cE.work = malloc(cnt * sizeof(int));
                                        memcpy(cE.work, input, cnt * sizeof(int));
                                        while(cA.halt != 1 && cB.halt != 1 && cC.halt != 1 && cD.halt != 1 && cE.halt != 1 && ) {
                                            cA.in = cE.out;
                                            compute(cA);
                                            cB.in = cA.out;
                                            compute(cB);
                                            cC.in = cB.out;
                                            compute(cC);
                                            cD.in = cC.out;
                                            compute(cD);
                                            cE.in = cD.out;
                                            compute(cE);
                                            if(cE.out > thrust)
                                                thrust = cE.out;
                                        }
                                    }

    printf("7b: %d\n\n", 0);

    free(input);
    input = NULL;
}


int compute(int param1, int param2) {

    int *work = malloc(cnt * sizeof(int));
    memcpy(work, input, cnt * sizeof(int));

    int step = 0;
    int output = 0;
    int paramIdx = 0;
    int param[2] = {param1,param2};

    for (int i = 0; i < cnt; i += step) {
        if(i<0 || i >= cnt) {
            printf("ERR %d\n",i);
            break;
        }
        int B = work[i] % 10000 / 1000;
        int A = work[i] % 1000 / 100;
        int op = work[i] % 100;
        int in1, in2;

        if ((op == 1 || op == 2 || (op > 4 && op < 9)) && A == 0)
            in1 = work[work[i + 1]];
        else
            in1 = work[i + 1];
        if ((op == 1 || op == 2 || (op > 4 && op < 9)) && B == 0)
            in2 = work[work[i + 2]];
        else
            in2 = work[i + 2];

        if (op == 1)
            work[work[i + 3]] = in1 + in2;
        if (op == 2)
            work[work[i + 3]] = in1 * in2;
        if (op == 3)
            work[work[i + 1]] = param[paramIdx++];
        if (op == 4)
            output = work[work[i + 1]];
        if (op == 5)
            if(in1 != 0)
                step = in2 - i;
            else
                step = 3;
        if (op == 6)
            if(in1 == 0)
                step = in2 - i;
            else
                step = 3;
        if (op == 7)
            work[work[i + 3]] = (in1 < in2) ? 1 : 0;
        if (op == 8)
            work[work[i + 3]] = (in1 == in2) ? 1 : 0;
        if (op == 99)
            break;

        switch(op){
            case 1:
            case 2:
            case 7:
            case 8: step = 4; break;
            case 3:
            case 4: step = 2; break;
        }
    }
    return output;
}