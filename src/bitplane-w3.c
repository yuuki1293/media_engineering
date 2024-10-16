/* bitplane-w.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
void hide_image( int n1, int n2 );

int main(void)
{
    printf("原画像（階調画像）を読み込みます\n");
    load_image( 0, "" ); /* ファイル → 画像No.0(原画像)   */
    printf("混入画像（２値画像(白:255, 黒:0)）の読み込み\n");
    load_image( 1, "" ); /* ファイル → 画像No.1(混入画像) */
    if ( width[0] != width[1] || height[0] != height[1] ){
        printf("画像のサイズは同一である必要があります\n");
        exit(1);
    }
    hide_image( 0, 1 );  /* No.0にNo.1を混入 */
    save_image( 0, "" ); /* 画像No.0 → ファイル */
    return 0;
}

void hide_image( int n1, int n2 )
/* 階調画像(No.n1)のLSBのビットプレーンに */
/* ２値画像(No.n2)を混入する関数          */
{
    int x,y;
    unsigned char bitmask;

    for(y=0;y<height[n1];y++){
        for(x=0;x<width[n1];x++){
            bitmask = (x + y) % 3 + 6;

            if ( image[n2][x][y] == 255 ) /* 白画素 */
                /* ビットごとの演算でLSBを1にする */
                /* bitmaskとのOR(|)を計算している */
                image[n1][x][y] = image[n1][x][y] | bitmask;
            else  /* 黒画素 */
                /* ビットごとの演算でLSBを0にする */
                /* bitmaskの否定(~)とのAND(&)を計算している */
                image[n1][x][y] = image[n1][x][y] & (~bitmask);
        }
    }
}
