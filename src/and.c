/* マスク画像で移動物体を抽出するプログラム extract.c */
#include<stdio.h>
#include<stdlib.h>
#include"pgmlib.h"

void and() {
    int x,y;

    for ( y = 0; y < height[1]; y ++ )
        for ( x = 0; x < width[1]; x ++ )
            image[2][x][y] &= image[1][x][y];
}

int main( )
{
    printf("*** 画像1のデータを読み込みます．\n");
    load_image(1,"" );        /* 画像1 --> image1      */
    printf("*** 画像2のデータを読み込みます．\n");
    load_image(2,"" );        /* 画像1 --> image1      */
    
    printf("*** AND画像のデータを保存します．\n");
    save_image(2,"" );        /* image2 --> 出力      */
    return 0;
}
