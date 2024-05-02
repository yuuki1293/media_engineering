/* ヒストグラム平滑化のプログラム smth_hst.c */
#include<stdio.h>
#include"pgmlib.h"
#define FINAL_LEVEL 64  /* 最終的な表現階調数 */

void smooth_histgram( )
/* image1 のヒストグラムを平滑化して image2 へ */
{
    long int hist1[GRAYLEVEL], hist2[GRAYLEVEL];
    int trans_table[GRAYLEVEL]; /* 濃度変換表 */
    int i, x, y; /* ループ変数 */
    long int target_value; /* 変換後の頻度の目標値 */          
    int gray; /* 階調用作業変数 */
    double gray_step; /* 表現階調間隔 */

    printf("ヒストグラムの平滑化を行います．\n");
    /* 原画像のヒストグラムを求める */
    for ( i = 0; i < GRAYLEVEL; i ++ ) hist1[i] = 0;
    for ( y = 0; y < height[0]; y ++ )
        for ( x = 0; x < width[0]; x ++ )
            hist1[ image[0][x][y] ] ++;
    /* ヒストグラム変換表を作る */
    target_value = (int)( width[0] * height[0] / (double)FINAL_LEVEL );
    for ( i = 0; i < FINAL_LEVEL; i ++ ) hist2[i] = 0;
    gray = 0;
    for ( i = 0; i < GRAYLEVEL; i ++ ){
        if ( abs( target_value - hist2[gray] ) <
             abs( target_value - ( hist2[gray] + hist1[i] ) ) ){
            gray ++;
            if ( gray >= FINAL_LEVEL ) gray = FINAL_LEVEL - 1;
        }
        trans_table[i] = gray;
        hist2[gray] = hist2[gray] + hist1[i];
    }
    /* ヒストグラムを平滑化した画像を image2 に代入 */
    width[1] = width[0];    height[1] = height[0];
    gray_step = (double)GRAYLEVEL / FINAL_LEVEL;
    for ( y = 0; y < height[1]; y ++ )
        for ( x = 0; x < width[1]; x ++ )
            image[1][y][x] = (unsigned char)
                ( trans_table[ image[0][y][x] ] * gray_step );
}

int main( )
{
    load_image( 0, ""); /* 原画像を image1 に読み込む */
    smooth_histgram( ); /* ヒストグラムを平滑化して image2 へ */
    save_image( 1, ""); /* image2 を保存する */
    return 0;
}
