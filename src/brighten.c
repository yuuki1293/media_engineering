#include <stdio.h>
#include "include/pgmlib.h"

void brighten(int n,  int shift);

int main(void)
{
    load_image(0, "lenna.pgm");
    brighten(0, 100);
    save_image(0, "out-bup.pgm");
    return 0;
}

void brighten(int n, int shift)
{
    int x, y, brightness;

    for (y = 0; y < height[n]; y++){
        for (x = 0; x < width[n]; x++)
        {
            brightness = image[n][x][y] + shift;
            if (brightness > 255) brightness = 255;
            image[n][x][y] = brightness;
        }
    }
}