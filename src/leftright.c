#include <stdio.h>
#include "include/pgmlib.h"

void lr_reverse(int n1, int n2);

int main(void)
{
    load_image(0, "lenna.pgm");
    lr_reverse(0, 1);
    save_image(1, "out-lr.pgm");
    return 0;
}

void lr_reverse(int n1, int n2)
{
    int x, y;

    width[n2] = width[n1]; height[n2] = height[n2] = height[n1];
    for (y = 0; y < height[n1]; y++){
        for (x = 0; x < width[n1]; x++){
            image[n2][x][y] = image[n1][width[n1]-1-x][y];
        }
    }
    
}