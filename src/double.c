#include <stdio.h>
#include "include/pgmlib.h"

void expansion(int n1, int n2);

int main()
{
    load_image(0, "lenna.pgm");
    expansion(0, 1);
    save_image(1, "out-2bai.pgm");
    return 0;
}

void expansion(int n1, int n2)
{
    width[n2] = width[n1] * 2;
    height[n2] = height[n1] * 2;

    for (size_t y = 0; y < height[n1]; y++){
        for (size_t x = 0; x < width[n1]; x++){
            unsigned char tone = image[n1][x][y];
            image[n2][x * 2][y * 2] = tone;
            image[n2][x * 2 + 1][y * 2] = tone;
            image[n2][x * 2][y * 2 + 1] = tone;
            image[n2][x * 2 + 1][y * 2 + 1] = tone;
        }
    }
    
    return;
}