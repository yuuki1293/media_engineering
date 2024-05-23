/* forsen.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
void Forsen( int n1, int n2 );

int main(void)
{
    load_image( 0, "" );   /* ファイル → 画像No.0 */
    Forsen( 0, 1 );        /* No.0->フィルタ->No.1 */  
    save_image( 1, "" );   /* 画像No.1 → ファイル */
    return 0;
}

void Forsen( int n1, int n2 )
/* 画像No.n1をForsenフィルタリングして画像No.n2へ */
{
    int x,y,value;

    /* 画像No.n2の画像サイズを入力 */
    width[n2]=width[n1];  height[n2]=height[n1];
    init_image( n2, 0 );  /* 画像No.n2を0で初期化 */
    /* フィルタリング（最右列・最下ラインは無視） */
    for(y=0;y<height[n1]-1;y++){
        for(x=0;x<width[n1]-1;x++){
            value = abs( image[n1][x][y] - image[n1][x+1][y+1] )
                + abs( image[n1][x][y+1] - image[n1][x+1][y] );
            image[n2][x][y] = value / 2;
        }
    }
}
