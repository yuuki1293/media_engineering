/* freq_filter2.c  このプログラムの名前 */
#include<stdio.h>
#include<math.h>
#include"pgmlib.h"
#include"fft.h"  /* FFT・IFFTの関数 */

/* 関数のプロトタイプ宣言 */
void make_original_data( int n );
void freq_filtering(void);
void make_output_image( int n );

int main(void)
{
    load_image( 0, "" );     /* 原画像 --> 画像No.0  */
    make_original_data( 0 ); /* 元のデータを作成する */
    FFT2( 1 );               /* ２次元 FFT の実行    */
    freq_filtering( );       /* 周波数フィルタリング */
    FFT2( -1 );              /* ２次元 逆FFT の実行  */
    make_output_image( 1 );  /* 画像No.1に画像を形成 */
    save_image( 1, "");      /* 画像No.1を出力する   */
    return 0;
}

void make_original_data( int n )
/* 画像No.nのデータを data[x][y], jdata[x][y] に代入する */
{
    int i,j;

    if ( width[n] != height[n] ){
        printf("縦と横の画素数が異なっているので終了します．\n");
        exit(1);
    } else {
        printf("\n読み込んだ画像を元データに直します．\n");
        num_of_data = width[n];
        for (j=0; j<num_of_data; j++){
            for (i=0; i<num_of_data; i++){
                 data[i][j] = (double)image[n][i][j];
                jdata[i][j] = 0.0;  /* 虚数部＝0 */
            }
        }
    }
}

void freq_filtering(void)
/* 周波数領域に対するフィルタリングを行う */
/* 例：低域通過フィルタ                   */
{
    int i,j,num2;

    printf("\nFFT後の係数に対するフィルタリングを行います．\n");
    /* フィルタリングを行う   */
    /* これは低域通過フィルタ */
    num2 = 20;
    for (j=0; j<num_of_data; j++){
        for (i=0; i<num_of_data; i++){
            if ( ( i > num2 ) && ( i < num_of_data - num2 ) ||
                 ( j > num2 ) && ( j < num_of_data - num2 ) ){
                 data[i][j] = 0.0;
                jdata[i][j] = 0.0;
            }
        }
    }
}

void make_output_image( int n )
/* データ data[y][x], jdata[y][x] を画像No.nに代入 */
{
    int x, y;

    printf("\n逆FFT変換後の画像を作成します．\n");
    width[n] = num_of_data;  height[n] = num_of_data;
    for(y=0; y<height[n]; y++){
        for(x=0; x<width[n]; x++){
            if ( data[x][y] < 0 ) data[x][y] = 0;
            if ( data[x][y] > MAX_BRIGHTNESS ) 
                     data[x][y] = MAX_BRIGHTNESS;
            image[n][x][y] = (unsigned char)data[x][y];
        }
    }
}
