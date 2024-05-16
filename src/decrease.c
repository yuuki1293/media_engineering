/* binarization.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
void decrease( int n );

int main(void)
{
    load_image( 0, "" );   /* ファイル → 画像No.0 */
    decrease( 0 );         /* 画像No.0を２値化     */
    save_image( 0, "" );   /* 画像No.0 → ファイル */
    return 0;
}

void decrease( int n)
/* 画像No.n をd値化する */
{
    int num, step = 0;
    int i, x, y;

    /* しきい値の入力 */
    do{
        printf("階調数（2-255）：");
        scanf("%d",&num);
    } while( num < 2 || num > 255 );

    /* 画像を２値化 */
    for(y=0;y<height[n];y++){
        for(x=0;x<width[n];x++){
            for (i = 0; image[n][x][y] >= 256. / num * i; i++);
            image[n][x][y] = 256. / num * (i - 1);
        }
    }
}
