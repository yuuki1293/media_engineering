/* binarization.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
void binarize( int n );

int main(void)
{
    load_image( 0, "" );   /* ファイル → 画像No.0 */
    binarize( 0 );         /* 画像No.0を２値化     */
    save_image( 0, "" );   /* 画像No.0 → ファイル */
    return 0;
}

void binarize( int n )
/* 画像No.n を２値化する */
{
    int thvalue;  /* ２値化のしきい値 */
    int x,y;

    /* しきい値の入力 */
    do{
        printf("２値化のしきい値（0-255）：");
        scanf("%d",&thvalue);
    } while( thvalue<0 || thvalue > 255 );
    /* 画像を２値化 */
    for(y=0;y<height[n];y++){
        for(x=0;x<width[n];x++){
            if ( image[n][x][y] <= thvalue ) image[n][x][y] = 0;
            else image[n][x][y] = 255;
        }
    }
}
