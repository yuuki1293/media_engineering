#include <stdio.h>
#include <inttypes.h>
#include "pgmlib.h"

void brighten(int n, int8_t shift);

int main(void)
{
    load_image(0, "lenna.pgm");
    int8_t shift;
    scanf("%" SCNd8, &shift);
    brighten(0, shift);

    char* out = malloc(20);
    sprintf(out, "lenna%+d.pgm", shift);
    save_image(0, out);
    return 0;
}

void brighten(int n, int8_t shift)
{
    int x, y, brightness;

    for (y = 0; y < height[n]; y++){
        for (x = 0; x < width[n]; x++)
        {
            brightness = image[n][x][y] + shift;
            if (brightness > 255) brightness = 255;
            if (brightness < 0) brightness = 0;
            image[n][x][y] = brightness;
        }
    }
}