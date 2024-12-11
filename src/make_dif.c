/* 画像間の差分画像を求めるプログラム make_dif.c */
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

void make_difference_image( )
{
    int x, y;        /* ループ変数   */
    int difference;  /* 差分の絶対値 */

    printf("frame1 と frame2 の画素ごとの階調値の差分の\n");
    printf("絶対値を求めて image[2][x][y] に代入します．\n");
    for ( y = 0; y < height[2]; y ++ ){
        for ( x = 0; x < width[2]; x ++ ){
            difference = abs( image[2][x][y] - image[1][x][y] );
            image[2][x][y] = (unsigned char)difference;
        }
    }
}

void make_mask_image( )
{
    int x, y;      /* ループ変数 */
    int max_dif;   /* 許容できる最大の差分値 */
    int xmin, xmax;    /* マスク領域の範囲 */

    printf("差分画像からマスク画像を作ります．\n");
    do {
        printf("差分の許容最大階調値 (1～100) : ");
        scanf("%d", &max_dif);
    } while ( max_dif < 1 || max_dif > 100 );
	
    /* マスク画像の生成 */
    for ( y = 0; y < height[2]; y ++ ){
        xmin = width[2];
        for ( x = 0; x < width[2]; x ++ )
            if ( image[2][x][y] > max_dif ){
                xmin = x;    break;
            }
        xmax = 0;
        for ( x = width[2] - 1; x >= 0; x -- )
            if ( image[2][x][y] > max_dif ){
                xmax = x;    break;
            }
        for ( x = 0; x < width[2]; x ++ )
            if ( x < xmin || x > xmax ) image[2][x][y] = 0;
            else image[2][x][y] = MAX_BRIGHTNESS;
    }

	/*
    printf("差分画像からマスク画像を作ります．\n");
    do {
        printf("差分の許容最大階調値 (1～100) : ");
        scanf("%d", &max_dif);
    } while ( max_dif < 1 || max_dif > 100 );
    // マスク画像の生成 
    for ( y = 0; y < height[2]; y ++ )
        for ( x = 0; x < width[2]; x ++ )
            if ( image[2][y][x] > max_dif ) 
                image[2][y][x] = MAX_BRIGHTNESS;
            else image[2][y][x] = 0;
	*/

}

main( )
{
    printf("*** frame No.1 のデータを読み込みます．\n");
    load_image(1,"" );        /* frame1 データ --> image1  */
    copy_image1_to_image2( );  /* frame1 データ --> image2  */
    printf("*** frame No.2 のデータを読み込みます．\n");
    load_image(2,"" );        /* frame2 データ --> image1  */
    make_difference_image( );  /* 差分を求めて image2 へ    */
    printf("*** 差分画像のデータを保存します．\n");
    save_image(2,"" );        /* 差分画像 --> 出力         */
    make_mask_image( );        /* 差分マスク画像 --> image2 */
    printf("*** 差分画像から作ったマスク画像を保存します．\n");
    save_image(2,"" );        /* マスク画像 --> 出力       */
    return 0;
}
