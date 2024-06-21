#include<stdio.h>
#include<math.h>
#include "FFT1.h"

int main() {
    double re[] = {0, 0, 1, 1, 1, 1, 0, 0};
    double im[] = {0, 0, 0, 0, 0, 0, 0, 0};

    FFT1(re, im, 8, 1);

    for (size_t i = 0; i < 8; i++)
    {
        printf("%lf%+lf\n", re[i], im[i]);
    }

    return 0;
}