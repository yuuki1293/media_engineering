/* マスク画像で移動物体を抽出するプログラム extract.c */
#include<stdio.h>
#include<stdlib.h>
#include"pgmlib.h"

void copy_image1_to_image2( )
/* image1 の画像データを image2 へコピーする */
{
    int x, y;  /* ループ変数 */

    width[2] = width[1];  height[2] = height[1];
    for ( y = 0; y < height[1]; y ++ )
        for ( x = 0; x < width[1]; x ++ )
            image[2][x][y] = image[1][x][y];
}

void extract_image( )
{
    int x, y;  /* ループ変数 */

    for ( y = 0; y < height[1]; y ++ )
        for ( x = 0; x < width[1]; x ++ )
            if ( image[1][x][y] == 0 ) image[2][x][y] = 0;
}

int main( )
{
    printf("*** 原画像のデータを読み込みます．\n");
    load_image(1,"" );        /* 画像1 --> image1      */
    copy_image1_to_image2( );  /* 画像1 --> image2      */
    printf("*** マスク画像のデータを読み込みます．\n");
    load_image(1,"" );        /* 画像2 --> image1      */
    extract_image( );          /* 領域抽出して image2 へ */
    printf("*** 抽出画像のデータを保存します．\n");
    save_image(2,"" );        /* image2 --> 出力      */
    return 0;
}
