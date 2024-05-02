/* gamma.c  このプログラムの名前 */
#include<stdio.h>
#include<math.h>
#include"pgmlib.h"

int trans_table[256];  /* 変換表（グラフ）*/

void trans( int n );
/* 画像No.nの画像を変換グラフで変換する */

int main(void)
{
    load_image( 0, "" );   /* ファイル → 画像No.0 */
    trans( 0 );            /* 画像No.0画像を濃度変換 */
    save_image( 0, "" );   /* 画像No.0 → ファイル */
    return 0;
}

void trans( int n )
/* 画像No.nの画像を変換グラフで変換する */
{
    int f;
    int x,y;
    double gamma;  /* γ値 */
    double work;

    do{
        printf("γ値(>0)を実数値で入力して下さい：");
        scanf("%lf",&gamma);
    } while( gamma <= 0 );
    work = 1.0 / gamma;
    /* 変換表（グラフ）の設定 */
    for(f=0;f<256;f++){
        trans_table[f] = (int)( 255.0 * pow( (double)f/255.0, work ));
        if (trans_table[f] > 255) trans_table[f]=255;
        else if (trans_table[f] < 0) trans_table[f]=0;
    }
    /* 各画素の階調値を変換表に従って変換 */
    for(y=0;y<height[n];y++){
        for(x=0;x<width[n];x++){
            image[n][x][y] = trans_table[ image[n][x][y] ];
        }
    }
}
