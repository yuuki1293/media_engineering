#include <stdio.h>
#include <limits.h>
#include "include/pgmlib.h"

void reverse(int n);

int main()
{
    load_image(0, "lenna.pgm");
    reverse(0);
    save_image(0, "out-rev.pgm");
    return 0;
}

void reverse(int n)
{
    for (size_t y = 0; y < height[n]; y++){
        for (size_t x = 0; x < width[n]; x++){
            image[n][x][y] = UCHAR_MAX - image[n][x][y];
        }
    }
    
    return;
}