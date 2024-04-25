#include <stdio.h>
#include "include/pgmlib.h"

void combine(int n1, int n2);

int main()
{
    load_image(0, "lenna.pgm");
    load_image(1, "barbara.pgm");
    combine(0, 1);
    save_image(0, "combine.pgm");
}

void combine(int n1, int n2)
{
    for (size_t y = 0; y < height[0]; y++){
        for (size_t x = 0; x < width[0]; x++){
            const unsigned char ave = (image[0][x][y] + image[1][x][y]) / 2;
            image[0][x][y] = ave;
        }
    }
}