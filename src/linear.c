#include<stdio.h>
#include<inttypes.h>
#include"pgmlib.h"
long int hist[256]; /* ヒストグラム用１次元配列 */
void make_histogram( int n );
void make_histogram_image( int h, int n );
void liner_image(int n, int m);

int main(void)
{
    load_image( 0, "" );   /* ファイル → 画像No.0 */
    make_histogram( 0 );    /* 画像No.0のヒストグラムの作成 */
    /* ヒストグラムの画像を作って画像No.1に保存 */
    make_histogram_image( 256, 1 );
    save_image( 1, "" );   /* 画像No.1 → ファイル */
    liner_image(0, 0);
    save_image(0, "");
    make_histogram(0);
    make_histogram_image(256, 1);
    save_image(1, "");
    return 0;
}

void make_histogram( int n )
/* 画像No.n を元にヒストグラムを作る */
{
    int i,x,y;
    
    /* ヒストグラムの初期化 */
    for(i=0;i<256;i++) hist[i] = 0;
    /* 画像をラスタスキャンしてグラフを作る */
    for(y=0;y<height[n];y++)
        for(x=0;x<width[n];x++)
            hist[ image[n][x][y] ]++; /* 累積を１増加 */
}

void make_histogram_image( int h, int n )
/* ヒストグラムを画像化(横256×縦h画素)して */
/* 画像No.nに記録する */
{
    int i,x,y;
    long int max; /* 最大頻度 */
    int takasa;   /* 各階調値の頻度の高さ */
    
    /* 画像の縦・横の値の入力 */
    width[n]=256;  height[n]=h;
    /* ヒストグラムの最大頻度maxを求める */
    max=0;
    for(i=0;i<256;i++)
        if ( hist[i] > max ) max = hist[i];
    /* 縦幅をh画素に正規化しながら画像に値を代入 */
    for(x=0;x<width[n];x++){
        takasa = (int)( h / (double)max * hist[x] );
        if ( takasa > h ) takasa = h;
        for(y=0;y<h;y++)
            if ( y < takasa ) image[n][x][h-1-y] = 0;
            else image[n][x][h-1-y] = 255;
    }
}

void liner_image(int n, int m)
{
    float fmin = UINT8_MAX;
    float fmax = 0;
    uint8_t g;
    int x, y;

    for (y = 0; y < height[n]; y++)
        for (x = 0; x < width[n]; x++)
        {
            float f = image[n][x][y];
            if (f < fmin)
                fmin = f;
            if (f > fmax)
                fmax = f;
        }

    printf("min: %d\nmax: %d\n", (uint8_t)fmin, (uint8_t)fmax);
    
    for (y = 0; y < height[n]; y++)
        for (x = 0; x < width[n]; x++)
        {
            float f = image[n][x][y];
            uint8_t g = (f - fmin) / (fmax - fmin) * 255.;
            image[m][x][y] = g;
        }
}