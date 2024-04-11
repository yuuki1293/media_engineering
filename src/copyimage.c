#include<stdio.h>
#include "include/pgmlib.h"

int main(void)
{
    load_image(0, "lenna.pgm");
    save_image(0, "out-cp.pgm");
    return 0;
}