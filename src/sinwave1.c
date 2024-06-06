/* sinwave1.c  このプログラムの名前 */
#include<stdio.h>
#include<math.h>
#include"pgmlib.h"
#define PI 3.14159265

/* 画像No.nに正弦波を加える */
void add_sin_waves( int n, double a0, double a, double Nx,
  double theta_x, double Ny, double theta_y );
/* 画像No.nを作る */
void make_original_image( int n );  

int main(void)
{
    make_original_image( 0 );    /* 画像No.0を作る */
    save_image( 0, "" );   /* 画像No.0 → ファイル */
    return 0;
}

void add_sin_waves( int n, double a0, double a, double Nx,
  double theta_x, double Ny, double theta_y )
/* 画像No.nに正弦波を加える */
{
    int x,y,brightness;
    double term_x,term_y,value;
    
    for(y=0;y<height[n];y++)
        for(x=0;x<width[n];x++){
            if ( Nx == 0 ) term_x = 0.0;
            else term_x = 2.0 * PI / Nx * ( x - theta_x );
            if ( Ny == 0 ) term_y = 0.0;
            else term_y = 2.0 * PI / Ny * ( y - theta_y );
            value = a0 + a * sin( term_x + term_y );
            brightness = image[n][x][y] + value;
            if ( brightness > 255 )    brightness = 255;
            else if ( brightness < 0 ) brightness = 0;
            image[n][x][y] = brightness;
        }
}

void make_original_image( int n )
/* 画像No.nを作る */
{
    /* 256×256画素，画素値0で初期化 */
    width[n] = 256;  height[n] = 256;
    init_image( n, 0);
    /* sin波を加える */
    /* (画像番号, a0, a, Nx, theta_x, Ny, theta_y) */
    add_sin_waves( n, 128.0, 128.0, 128.0, 0.0, 0.0, 0.0 );
}
