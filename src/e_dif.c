/* e_dif.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
void error_diffusion( int n );
int work[MAX_IMAGESIZE][MAX_IMAGESIZE]; /* 作業用配列 */

int main(void)
{
    load_image( 0, "" );   /* ファイル → 画像No.0 */
    error_diffusion( 0 );  /* 画像No.0を誤差拡散で２値化 */
    printf("変換後: %d\n", image[0][100][80]);
    save_image( 0, "" );   /* 画像No.0 → ファイル */
    return 0;
}

void error_diffusion( int n )
/* 画像No.n を誤差拡散法で２値化する */
{
    int x,y,error;
    
    /* 作業用配列へのコピー */
    for(y=0;y<height[n];y++)
        for(x=0;x<width[n];x++)
            work[x][y] = image[n][x][y];
    /* 誤差拡散（簡易版：画像の周囲の画素は処理していない） */
    for(y=0;y<height[n]-1;y++){
        for(x=1;x<width[n]-1;x++){
            /* 黒白どちらの画素にするかを決定 */
            if ( (work[x][y] - 0) <= (255 - work[x][y] ) ){
                error = work[x][y] - 0;
                work[x][y] = 0;   /* 黒画素に決定 */
            } else {
                error = work[x][y] - 255; /* 値は0以下 */
                work[x][y] = 255; /* 白画素に決定 */
            }
            /* 周囲の画素への誤差の分散 */
            work[x+1][y]   += (int)(0.3 * error);
            work[x+1][y+1] += (int)(0.2 * error);
            work[x][y+1]   += (int)(0.3 * error);
            work[x-1][y+1] += (int)(0.2 * error);
        }
    }
    /* 最終的な画像の生成（周囲１画素は白画素にしている） */
    init_image( 0, 255 ); /* 画像No.0を全て白画素で初期化 */
    for(y=0;y<height[n]-1;y++)
        for(x=1;x<width[n]-1;x++)
            if ( work[x][y] > 255 ) image[n][x][y] = 255;
            else if ( work[x][y] < 0 ) image[n][x][y] = 0;
            else image[n][x][y] = work[x][y];
}
