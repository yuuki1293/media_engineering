/* binarization.c  このプログラムの名前 */
#include <stdio.h>
#include "pgmlib.h"

long int hist[256]; /* ヒストグラム用１次元配列 */
long int hist2[256];

void binarize(int n);
void make_histogram(int n);
void make_histogram_image(int h, int n);

int main(void)
{
    load_image(0, "textdata.pgm"); /* ファイル → 画像No.0 */
    make_histogram(0);
    make_histogram_image(255, 1);
    save_image(1, "textdata-hist.pgm");
    binarize(0);       /* 画像No.0を２値化     */
    save_image(0, "textdata-bin.pgm"); /* 画像No.0 → ファイル */

    int count;
    for (int y = 0; y < height[0]; y++)
        for (int x = 0; x < width[0]; x++)
            if (image[0][x][y] == 255)
                count++;
    printf("%d\n", count);

    return 0;
}

void binarize(int n)
/* 画像No.n を２値化する */
{
    int x, y;

    /* 画像を２値化 */
    for (y = 0; y < height[n]; y++)
    {
        for (x = 0; x < width[n]; x++)
        {
            if (image[n][x][y] <= 120)
                image[n][x][y] = 0;
            else
                image[n][x][y] = 255;
        }
    }
}

void make_histogram(int n)
/* 画像No.n を元にヒストグラムを作る */
{
    int i, x, y;

    /* ヒストグラムの初期化 */
    for (i = 0; i < 256; i++)
        hist2[i] = 0;
    /* 画像をラスタスキャンしてグラフを作る */
    for (y = 0; y < height[n]; y++)
        for (x = 0; x < width[n]; x++)
            hist2[image[n][x][y]]++; /* 累積を１増加 */

    /* ヒストグラム平滑化 */
    for (i = 0; i < 255; i++)
        hist[i] = (hist2[i - 1] + hist2[i] + hist2[i + 1]) / 3;
}

void make_histogram_image(int h, int n)
/* ヒストグラムを画像化(横256×縦h画素)して */
/* 画像No.nに記録する */
{
    int i, x, y;
    long int max; /* 最大頻度 */
    int takasa;   /* 各階調値の頻度の高さ */

    /* 画像の縦・横の値の入力 */
    width[n] = 256;
    height[n] = h;
    /* ヒストグラムの最大頻度maxを求める */
    max = 0;
    for (i = 0; i < 256; i++)
        if (hist[i] > max)
            max = hist[i];
    /* 縦幅をh画素に正規化しながら画像に値を代入 */
    for (x = 0; x < width[n]; x++)
    {
        takasa = (int)(h / (double)max * hist[x]);
        if (takasa > h)
            takasa = h;
        for (y = 0; y < h; y++)
            if (y < takasa)
                image[n][x][h - 1 - y] = 0;
            else
                image[n][x][h - 1 - y] = 255;
    }
}
