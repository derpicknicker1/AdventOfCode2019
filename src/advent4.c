/*
 --- Day 4: Secure Container ---
 You arrive at the Venus fuel depot only to discover it's protected by a password. The Elves had written the password on a sticky note, but someone threw it out.

 However, they do remember a few key facts about the password:

 It is a six-digit number.
 The value is within the range given in your puzzle input.
 Two adjacent digits are the same (like 22 in 122345).
 Going from left to right, the digits never decrease; they only ever increase or stay the same (like 111123 or 135679).
 Other than the range rule, the following are true:

 111111 meets these criteria (double 11, never decreases).
 223450 does not meet these criteria (decreasing pair of digits 50).
 123789 does not meet these criteria (no double).
 How many different passwords within the range given in your puzzle input meet these criteria?

 Your puzzle answer was 1955.

--- Part Two ---
 An Elf just remembered one more important detail: the two adjacent matching digits are not part of a larger group of matching digits.

 Given this additional criterion, but still ignoring the range rule, the following are now true:

 112233 meets these criteria because the digits never decrease and all repeated digits are exactly two digits long.
 123444 no longer meets the criteria (the repeated 44 is part of a larger group of 444).
 111122 meets the criteria (even though 1 is repeated more than twice, it still contains a double 22).
 How many different passwords within the range given in your puzzle input meet all of the criteria?

 Your puzzle answer was 1319.
*/

#include "advent.h"

static int begin = 0;
static int end = 0;

/*##########################
# Get input data from file #
##########################*/
static int getInput(char *f) {
    char *line = NULL;
    size_t l = 0;
    begin = 0;
    end = 0;

    FILE *file = fopen(f, "r");
    if (file == NULL) {
        printf("ERR: CAN NOT OPEN '%s'\n\n", f);
        return 0;
    }

    while (getline(&line, &l, file) != -1)
        sscanf(line, "%d-%d", &begin, &end);

    free(line);
    fclose(file);
    return 1;
}

/*##########################
# Function to solve part A #
##########################*/
void get4a(char * f) {
    if (!getInput(f))
        return;

    int cnt = 0;
    for (int i = begin; i <= end; i++) {

        int divisor = 100000;
        int last = -1;
        int two = 0;
        int match = 1;
        int value = i;
        for (int j = 0; j < 6; j++){
            int digit = value / divisor;
            if (digit == last)
                two = 1;
            if (digit < last)
                match = 0;
            last = digit;
            value = value - (digit * divisor);
            divisor /= 10;
        }
        if (match && two)
            cnt++;
    }

    printf("4a: %d\n", cnt);
}


/*##########################
# Function to solve part B #
##########################*/
void get4b(char *f) {

    if (!getInput(f))
        return;

    int cnt = 0;
    for (int i = begin; i <= end; i++) {
        int divisor = 100000;
        int last = -1;
        int two = 0;
        int found_two = 0;
        int match = 1;
        int value = i;
        for (int j = 0; j < 6; j++) {
            int digit = value / divisor;
            if (digit == last)
                two++;
            else {
                if (two == 1)
                    found_two = 1;
                two = 0;
            }
            if (digit < last)
                match = 0;
            last = digit;
            value = value - (digit * divisor);
            divisor /= 10;
        }
        if (match && (found_two || two == 1))
            cnt++;
    }

    printf("4b: %d\n\n", cnt);
}
