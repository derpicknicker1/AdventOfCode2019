/*
--- Day 6: Universal Orbit Map ---
You've landed at the Universal Orbit Map facility on Mercury. Because navigation in space often involves transferring between orbits, the orbit maps here are useful for finding efficient routes between, for example, you and Santa. You download a map of the local orbits (your puzzle input).

Except for the universal Center of Mass (COM), every object in space is in orbit around exactly one other object. An orbit looks roughly like this:

                  \
                   \
                    |
                    |
AAA--> o            o <--BBB
                    |
                    |
                   /
                  /
In this diagram, the object BBB is in orbit around AAA. The path that BBB takes around AAA (drawn with lines) is only partly shown. In the map data, this orbital relationship is written AAA)BBB, which means "BBB is in orbit around AAA".

Before you use your map data to plot a course, you need to make sure it wasn't corrupted during the download. To verify maps, the Universal Orbit Map facility uses orbit count checksums - the total number of direct orbits (like the one shown above) and indirect orbits.

Whenever A orbits B and B orbits C, then A indirectly orbits C. This chain can be any number of objects long: if A orbits B, B orbits C, and C orbits D, then A indirectly orbits D.

For example, suppose you have the following map:

COM)B
B)C
C)D
D)E
E)F
B)G
G)H
D)I
E)J
J)K
K)L
Visually, the above map of orbits looks like this:

        G - H       J - K - L
       /           /
COM - B - C - D - E - F
               \
                I
In this visual representation, when two objects are connected by a line, the one on the right directly orbits the one on the left.

Here, we can count the total number of orbits as follows:

D directly orbits C and indirectly orbits B and COM, a total of 3 orbits.
L directly orbits K and indirectly orbits J, E, D, C, B, and COM, a total of 7 orbits.
COM orbits nothing.
The total number of direct and indirect orbits in this example is 42.

What is the total number of direct and indirect orbits in your map data?

Your puzzle answer was 402879.

--- Part Two ---
Now, you just need to figure out how many orbital transfers you (YOU) need to take to get to Santa (SAN).

You start at the object YOU are orbiting; your destination is the object SAN is orbiting. An orbital transfer lets you move from any object to an object orbiting or orbited by that object.

For example, suppose you have the following map:

COM)B
B)C
C)D
D)E
E)F
B)G
G)H
D)I
E)J
J)K
K)L
K)YOU
I)SAN
Visually, the above map of orbits looks like this:

                          YOU
                         /
        G - H       J - K - L
       /           /
COM - B - C - D - E - F
               \
                I - SAN
In this example, YOU are in orbit around K, and SAN is in orbit around I. To move from K to I, a minimum of 4 orbital transfers are required:

K to J
J to E
E to D
D to I
Afterward, the map of orbits looks like this:

        G - H       J - K - L
       /           /
COM - B - C - D - E - F
               \
                I - SAN
                 \
                  YOU
What is the minimum number of orbital transfers required to move from the object YOU are orbiting to the object SAN is orbiting? (Between the objects they are orbiting - not between YOU and SAN.)

Your puzzle answer was 484.
*/

#include "advent.h"

int strToId(char* in);

static int planets[353536] = {};

/*##########################
# Get input data from file #
##########################*/
static int getInput(char *f) {
    char *line = NULL;
    size_t l = 0;

    FILE *file = fopen(f, "r");
    if (file == NULL) {
        printf("ERR: CAN NOT OPEN '%s'\n\n", f);
        return 0;
    }

    while (getline(&line, &l, file) != -1) {

        char a[3], b[3];
        sscanf(line,"%c%c%c)%c%c%c",&a[0],&a[1],&a[2],&b[0],&b[1],&b[2]);  

        int src = strToId(a);
        int dest = strToId(b); 
        planets[dest] = src;   
    }

    free(line);
    fclose(file);
    return 1;
}


/*##########################
# Function to solve part A #
##########################*/
void get6a(char * f) {

    if (!getInput(f))
        return;

    int cnt = 0;
    for(int i = 0; i < 353536; i++) {
        if(planets[i] != 0) {
            int pos = i;
            cnt++;
            while(planets[pos] != strToId("COM")){
                cnt++;
                pos = planets[pos];
            }
        }
    }
    
    printf("6a: %d\n", cnt);

}


/*##########################
# Function to solve part B #
##########################*/
void get6b(char *f) {

    if (!getInput(f))
        return;

    int cnt = 0, cnt2 = 0;
    int you = planets[strToId("YOU")];
    int youPos = you;
    int sanPos = planets[strToId("SAN")];

    while(youPos != sanPos){
        cnt2 = 0;
        while(youPos != strToId("COM") && youPos != sanPos) {
            youPos = planets[youPos];
            cnt2++;
        }

        if(youPos != sanPos) {
            sanPos = planets[sanPos];
            youPos = you;
            cnt++;
        }
    }

    printf("6b: %d\n\n", cnt+cnt2);
}


int strToId(char* in) {
    int res = 0;
    int mul = 1;
    for(int i = 2; i >= 0; i--) {
        int val = 0;
        if(in[i] >= 'A')
            val += (in[i] - 65);
        else
            val += (in[i] - 48 + 26);

        res += val * mul;
        mul *= 100;
    }
    return res;
}