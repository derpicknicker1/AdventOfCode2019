/*

*/

#include "advent.h"

typedef struct Moon
{
    int x[3], v[3];
} Moon;

typedef struct DataSet
{
    int x[4], v[4];
} DataSet;

static Moon moons[4];

int timeStep();
DataSet getDataSet(int x);
int compareDataSet(DataSet a, DataSet b);
int ggt(int a, int b);

/*##########################
# Get input data from file #
##########################*/
static int getInput(char *f) {
    char *line = NULL;
    size_t l = 0;
    int m = 0;


    FILE *file = fopen(f, "r");
    if (file == NULL)
    {
        printf("ERR: CAN NOT OPEN '%s'\n\n", f);
        return 0;
    }

    while (getline(&line, &l, file) != -1)
    {
        sscanf(line, "<x=%d, y=%d, z=%d>", &moons[m].x[0], &moons[m].x[1], &moons[m].x[2]);
        m++;
    }

    free(line);
    fclose(file);
    return 1;
}


/*##########################
# Function to solve part A #
##########################*/
void get12a(char * f) {

    if (!getInput(f))
        return;

    int energy = 0;

    for (int i = 0; i < 1000; i++)
        energy = timeStep();


    printf("12a: %d\n", energy);
}


/*##########################
# Function to solve part B #
##########################*/
void get12b(char *f) {

    if (!getInput(f))
        return;

    DataSet ds0[3];
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            ds0[x].x[y] = moons[y].x[x];
            ds0[x].v[y] = moons[y].v[x];

        }
    }

    int i = 0;
    int j = 0;
    int cycle[3] = { };
    int done[3] = { };

    while (j < 3)
    {
        timeStep();
        i++;
        for (int x = 0; x < 3; x++)
        {
            DataSet ds = getDataSet(x);

            if (done[x] != 1 && compareDataSet(ds, ds0[x]))
            {
                j++;
                printf("found cycle for %d = %d\n", x, i);
                cycle[x] = i;
                done[x] = 1;
            }
        }
    }

    printf("ggt(%d,%d) = %d\n", cycle[0], cycle[1], ggt(cycle[0], cycle[1]));
    printf("ggt(%d,%d) = %d\n", cycle[0], cycle[2], ggt(cycle[0], cycle[2]));
    printf("ggt(%d,%d) = %d\n", cycle[2], cycle[1], ggt(cycle[2], cycle[1]));

    printf("12b: %d\n\n", 0);
}

int compareDataSet(DataSet a, DataSet b)
{
if (a.x[0] == b.x[0] && a.x[1] == b.x[1] && a.x[2] == b.x[2] && a.x[3] == b.x[3] && a.v[0] == b.v[0] && a.v[1] == b.v[1]
        && a.v[2] == b.v[2] && a.v[3] == b.v[3])
    return 1;
return 0;
}

DataSet getDataSet(int x)
{
    DataSet d;
    for (int y = 0; y < 4; y++)
    {
        d.x[y] = moons[y].x[x];
        d.v[y] = moons[y].v[x];

    }
    return d;
}

int timeStep()
{
    int energy = 0;
    for (int curr = 0; curr < 4; curr++)
        for (int other = 0; other < 4; other++)
            if (curr != other)
                for (int j = 0; j < 3; j++)
                    moons[curr].v[j] += (moons[other].x[j] > moons[curr].x[j]) ? 1 :
                                        (moons[other].x[j] < moons[curr].x[j]) ? -1 : 0;

    for (int curr = 0; curr < 4; curr++)
    {
        int pot = 0;
        int kin = 0;
        for (int j = 0; j < 3; j++)
        {
            moons[curr].x[j] += moons[curr].v[j];
            pot += abs(moons[curr].x[j]);
            kin += abs(moons[curr].v[j]);
        }

        energy += pot * kin;
    }
    return energy;
}

int ggt(int a, int b)
{
    int r;
    do
    {
        r = a % b;
        a = b;
        b = r;
    }
    while (r > 0);
    return a;
}
