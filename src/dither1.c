/* dither1.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
int dmatrix[4][4] = {
    {  0,  8,  2, 10 },
    { 12,  4, 14,  6 },
    {  3, 11,  1,  9 },
    { 15,  7, 13,  5 } };
void dither( int n );

int main(void)
{
    load_image( 0, "" );   /* ファイル → 画像No.0 */
    dither( 0 );           /* 画像No.0をディザ法で２値化 */
    save_image( 0, "" );   /* 画像No.0 → ファイル */
    return 0;
}

void dither( int n )
/* 画像No.nの階調画像をディザ法で２値化 */
{
    int x,y,i,j,xp,yp;
    int bxsize,bysize;  /* ブロック数（横・縦） */

    /* 原画像のサイズの確認 */
    if ( ( height[n] % 4 )!=0 || (width[n] % 4 )!=0 ){
        printf("画像のサイズが４の倍数ではありません．\n");
        exit(1);
    }
    /* 原画像を16段階の階調数で表現 */
    for(y=0;y<height[n];y++)
        for(x=0;x<width[n];x++)
            image[n][x][y] = image[n][x][y] / 16;
    /* ディザ法で２値化 */
    bxsize = width[n] / 4;  /* マトリクスが4×4なので */
    bysize = height[n] / 4;
    for(y=0;y<bysize;y++){
        for(x=0;x<bxsize;x++){
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    xp = x*4 + j;  /* 注目画素のx座標 */
                    yp = y*4 + i;  /* 注目画素のy座標 */
                    if ( image[n][xp][yp] <= dmatrix[i][j] )
                        image[n][xp][yp] = 0;
                    else
                        image[n][xp][yp] = 255;
                }
            }
        }
    }
}
