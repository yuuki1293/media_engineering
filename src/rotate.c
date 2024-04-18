#include <stdio.h>
#include "include/pgmlib.h"

void lr_rotate(int n1, int n2);

int main()
{
    load_image(0, "lenna.pgm");
    lr_rotate(0, 1);
    save_image(1, "out-rot.pgm");
    return 0;
}

void lr_rotate(int n1, int n2)
{
    width[n2] = height[n1];
    height[n2] = width[n2];
    for (size_t y = 0; y < height[n1]; y++){
        for (size_t x = 0; x < width[n1]; x++){
            image[n2][y][x] = image[n1][width[n1]-1-x][height[n1]-1-y];
        }
    }

    return;
}