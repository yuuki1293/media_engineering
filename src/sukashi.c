/* 電子透かしのプログラム sukashi.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"fft.h"
#include"pgmlib.h"

#define RATIO  0.1;  /* 混入させる画像に掛ける係数（＝ε） */

// int num_of_data;          /* データの要素番号の最大値 */
double  data2[FFT_MAX][FFT_MAX];   /* 元データの実数部 */
double jdata2[FFT_MAX][FFT_MAX];   /* 元データの虚数部 */

void make_data(int n )
/* image[n][y][x] のデータを data[y][x], jdata[y][x] に代入する */
{
    int i, j; /* ループ変数 */

    num_of_data = width[n];
    for ( i = 0; i < num_of_data; i ++ ){
        for ( j = 0; j < num_of_data; j ++ ){
             data[i][j] = (double)image[n][i][j];
            jdata[i][j] = 0.0;
        }
    }
}

void make_output_image( )
/* データ data[y][x], jdata[y][x] を image[y][x] に直す */
{
    int x, y; /* ループ変数 */

    width[2] = num_of_data;    height[2] = num_of_data;
    for ( y = 0; y < height[2]; y ++ ){
        for ( x = 0; x < width[2]; x ++ ){
            if ( data[x][y] < 0.0 ) image[2][x][y] = 0;
            else if ( data[x][y] > 255.0 ) image[2][x][y] = 255;
            else image[2][x][y] = (unsigned char)data[x][y];
        }
    }
}

void copy_image_data( )
/* image[0][y][x] ==> image[1][y][x] へデータコピー */
{
    int i, j; /* ループ変数 */

    for ( i = 0; i < width[0]; i ++ ){
        for ( j = 0; j < height[0]; j ++ ){
            image[1][i][j] = image[0][i][j];
        }
    }
    height[1] = height[0];  width[1] = width[0];
}

void copy_FFT_data( )
/* data, jdata ==> data2, jdata2 のデータコピー */
{
    int i, j; /* ループ変数 */

    for ( i = 0; i < num_of_data; i ++ ){
        for ( j = 0; j < num_of_data; j ++ ){
             data2[j][i] =  data[j][i];
            jdata2[j][i] = jdata[j][i];
        }
    }
}

void coding( )
/* 基準画像と混入する画像を指定して透かし入りの画像を保存する */
{
    int i, j; /* ループ変数 */

    printf("\n★　混入する画像（情報）を読み込みます．\n");
    load_image(0,"" );
//    copy_image_data();
	
    printf("\n★　基準画像（オリジナル）を読み込みます．\n");
    load_image(1,"" );
    make_data(1);
	
//    FFT2( data, jdata, num_of_data, 1 );
    FFT2( 1 );
    for ( i = 0; i < width[0]; i ++ ){
        for ( j = 0; j < height[0]; j ++ ){
            data[i][j] = data[i][j] + image[0][i][j] / RATIO;
        }
    }
//    FFT2( data, jdata, num_of_data, -1 ); /* ２次元 逆FFT の実行 */
    FFT2( -1 ); /* ２次元 逆FFT の実行 */
    make_output_image( );            /* 周波数データを画像に直す */
    printf("\n★　透かし入りの画像を保存します．\n");
	
    save_image(2,"" );                        /* 画像を保存する */
}

void decoding( )
/* 透かし入りの画像と基準画像を指定して抽出された画像を保存する */
{
    int i, j; /* ループ変数 */
    double d; /* 作業変数 */

    printf("\n★　基準画像（オリジナル）を読み込みます．\n");
    load_image(0,"" );
    make_data(0);
	
//    FFT2( data, jdata, num_of_data, 1 );
    FFT2( 1 );
    copy_FFT_data( );
	
    printf("\n★　透かし入り画像を読み込みます．\n");
    load_image(1,"" );
    make_data(1);
	
//    FFT2( data, jdata, num_of_data, 1 );
    FFT2( 1 );
	
	
    width[2] = width[0];  height[2] = height[0];
    for ( i = 0; i < width[2]; i ++ ){
        for ( j = 0; j < height[2]; j ++ ){
            d = ( data[i][j] - data2[i][j] ) * RATIO;
            if ( d < 0.0 ) image[2][i][j] = 0;
            else if ( d > MAX_BRIGHTNESS ) 
                image[2][i][j] = MAX_BRIGHTNESS;
            else image[2][i][j] = (unsigned char)d;
        }
    } 
    printf("\n★　抽出された画像（情報）を保存します．\n");
    save_image(2,"" );     /* 画像を保存する */
}

main( )
{
    int mode;

    printf("*** 電子透かしのプログラム ***\n");
    do {
        printf("透かしを入れる(1) ／ 透かしを取る(-1) : ");
        scanf("%d",&mode); 
    } while ( mode != 1 && mode != -1 );
    if ( mode == 1 ) coding( );
    else decoding( );
    return 0;
}
