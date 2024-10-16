/* bitplane-r.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
void extract_image( int n1, int n2 );

int main(void)
{
    printf("透かし入りの原画像（階調画像）を読み込みます\n");
    load_image( 0, "" ); /* ファイル → 画像No.0(原画像)   */
    extract_image( 0, 1 );  /* No.0から透かしを抽出してNo.1へ */
    save_image( 1, "" ); /* 画像No.1 → ファイル */
    return 0;
}

void extract_image( int n1, int n2 )
/* 階調画像(No.n1)のLSBのビットプレーンを抽出した */
/* ２値画像をNo.n2に作る                          */
{
    int x,y;
    unsigned char bitmask;

    width[n2]  = width[n1];
    height[n2] = height[n1];
    for(y=0;y<height[n1];y++){
        for(x=0;x<width[n1];x++){
            bitmask = 0b10000000 >> ((x + y) % 3 + 5);

            if ( image[n1][x][y] & bitmask ) /* 白画素 */
                image[n2][x][y] = 255;
            else /* 黒画素 */
                image[n2][x][y] = 0;
        }
    }
}
