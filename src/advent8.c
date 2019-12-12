/*
 --- Day 8: Space Image Format ---
 The Elves' spirits are lifted when they realize you have an opportunity to reboot one of their Mars rovers, and so they are curious if you would spend a brief sojourn on Mars. You land your ship near the rover.

 When you reach the rover, you discover that it's already in the process of rebooting! It's just waiting for someone to enter a BIOS password. The Elf responsible for the rover takes a picture of the password (your puzzle input) and sends it to you via the Digital Sending Network.

 Unfortunately, images sent via the Digital Sending Network aren't encoded with any normal encoding; instead, they're encoded in a special Space Image Format. None of the Elves seem to remember why this is the case. They send you the instructions to decode it.

 Images are sent as a series of digits that each represent the color of a single pixel. The digits fill each row of the image left-to-right, then move downward to the next row, filling rows top-to-bottom until every pixel of the image is filled.

 Each image actually consists of a series of identically-sized layers that are filled in this way. So, the first digit corresponds to the top-left pixel of the first layer, the second digit corresponds to the pixel to the right of that on the same layer, and so on until the last digit, which corresponds to the bottom-right pixel of the last layer.

 For example, given an image 3 pixels wide and 2 pixels tall, the image data 123456789012 corresponds to the following image layers:

 Layer 1: 123
 456

 Layer 2: 789
 012
 The image you received is 25 pixels wide and 6 pixels tall.

 To make sure the image wasn't corrupted during transmission, the Elves would like you to find the layer that contains the fewest 0 digits. On that layer, what is the number of 1 digits multiplied by the number of 2 digits?

 Your puzzle answer was 1935.

 --- Part Two ---
 Now you're ready to decode the image. The image is rendered by stacking the layers and aligning the pixels with the same positions in each layer. The digits indicate the color of the corresponding pixel: 0 is black, 1 is white, and 2 is transparent.

 The layers are rendered with the first layer in front and the last layer in back. So, if a given position has a transparent pixel in the first and second layers, a black pixel in the third layer, and a white pixel in the fourth layer, the final image would have a black pixel at that position.

 For example, given an image 2 pixels wide and 2 pixels tall, the image data 0222112222120000 corresponds to the following image layers:

 Layer 1: 02
 22

 Layer 2: 11
 22

 Layer 3: 22
 12

 Layer 4: 00
 00
 Then, the full image can be found by determining the top visible pixel in each position:

 The top-left pixel is black because the top layer is 0.
 The top-right pixel is white because the top layer is 2 (transparent), but the second layer is 1.
 The bottom-left pixel is white because the top two layers are 2, but the third layer is 1.
 The bottom-right pixel is black because the only visible pixel in that position is 0 (from layer 4).
 So, the final image looks like this:

 01
 10
 What message is produced after decoding your image?

 Your puzzle answer was CFLUL.
*/

#include "advent.h"

#define WIDTH 25
#define HEIGHT 6
#define LAYERSIZE (WIDTH*HEIGHT)

static char *input = NULL;
int cnt = 0;


/*##########################
# Get input data from file #
##########################*/
static int getInput(char *f) {
    char *line = NULL;
    size_t l = 0;
    cnt = 0;

    FILE *file = fopen(f, "r");
    if (file == NULL)
    {
        printf("ERR: CAN NOT OPEN '%s'\n\n", f);
        return 0;
    }
    cnt = getline(&line, &l, file);
    if (cnt != -1)
    {

        input = realloc(input, cnt * sizeof(char));
        memcpy(input, line, cnt * sizeof(char));
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
void get8a(char * f) {

    if (!getInput(f))
        return;

    int pos = 0;
    int counter[150][3] = { };
    int smallest = 0;

    while (pos < cnt)
        for (int start = pos; pos < start + LAYERSIZE; pos++)
            counter[start / LAYERSIZE][input[pos] - 48]++;


    for (int i = 0; i < pos / LAYERSIZE; i++)
        if (counter[i][0] < counter[smallest][0])
            smallest = i;

    printf("8a: %d\n", counter[smallest][1] * counter[smallest][2]);

    free(input);
    input = NULL;
}


/*##########################
# Function to solve part B #
##########################*/
void get8b(char *f) {

    if (!getInput(f))
        return;

    int pos = 0;
    int image[LAYERSIZE] = { };

    for (int i = 0; i < LAYERSIZE; i++)
        image[i] = 2;

    while (pos < cnt)
        for (int start = pos; pos < start + LAYERSIZE; pos++)
            if (image[pos - start] == 2 && (input[pos] - 48) != 2)
                image[pos - start] = input[pos] - 48;

    printf("8b:\n");

    for (int y = 0; y < HEIGHT; y++, printf("\n"))
        for(int x = 0; x < WIDTH; x++)
            printf("%c", image[y * WIDTH + x] ? '#' : ' ');


    printf("\n");

    free(input);
    input = NULL;
}
