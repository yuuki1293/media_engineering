/* freq_filter1.c  このプログラムの名前 */
#include <stdio.h>
#include <math.h>
#include "pgmlib.h"
#include "fft.h" /* FFT・IFFTの関数（付録C参照） */

/* 関数のプロトタイプ宣言 */
void make_original_data(int n);
void freq_filtering(void);
void make_output_image(int n);

int main(void)
{

    load_image(0, "");     /* 原画像 --> 画像No.0  */
    make_original_data(0); /* 元のデータを作成する */
    FFT2(1);               /* ２次元 FFT の実行    */
    freq_filtering();      /* 周波数フィルタリング */

    save_image(2, "");

    FFT2(-1);             /* ２次元 逆FFT の実行  */
    make_output_image(1); /* 画像No.1に画像を形成 */
    save_image(1, "");    /* 画像No.1を出力する   */
    return 0;
}

void make_original_data(int n)
/* 画像No.nのデータを data[x][y], jdata[x][y] に代入する */
{
    int i, j;

    if (width[n] != height[n])
    {
        printf("縦と横の画素数が異なっています．\n");
        exit(1);
    }
    else
    {
        printf("\n読み込んだ画像を元データに直します．\n");
        num_of_data = width[n];
        for (j = 0; j < num_of_data; j++)
            for (i = 0; i < num_of_data; i++)
            {
                data[i][j] = (double)image[n][i][j];
                jdata[i][j] = 0.0; /* 虚数部＝0 */
            }
    }
}

void freq_filtering(void)
/* 周波数領域に対するフィルタリングを行う */
{
    int x, y;
    int x_off, y_off;
    int x_max, y_max;
    float da, damax;
    float scale;

    width[2] = num_of_data;
    height[2] = num_of_data;
    x_off = num_of_data / 2;
    y_off = num_of_data / 2;

    printf("\nフィルタリングを行います．\n");
    printf("空間周波数用ファイル名を入れてください。．\n");
    /* フィルタリングを行う．実際は何もしていない */

    ////////////////////////////////////////////////////////////////////////

    damax = -1.0e10;
    scale = 200.0;

    for (y = 0; y < height[2]; y++)
    {
        for (x = 0; x < width[2]; x++)
        {
            da = scale * log10(sqrt(data[x][y] * data[x][y] + jdata[x][y] * jdata[x][y]));

            if (da > damax)
            {
                damax = da;
                x_max = x;
                y_max = y;
            }
        }
    }
    printf("max data %lf, x = %d, y = %d\n", damax, x_max, y_max);
    //			 printf("begin\n");

    for (y = 0; y < height[2] / 2; y++)
    {
        for (x = 0; x < width[2] / 2; x++)
        {
            da = scale * log10(sqrt(data[y][x] * data[y][x] + jdata[y][x] * jdata[y][x]));

            //             printf("max data %lf\n",da);
            if ((MAX_BRIGHTNESS + da - damax) < 0)
                image[2][y + y_off][x + x_off] = 0;
            //            else if ( da > MAX_BRIGHTNESS )
            //                       image3[y+y_off][x+x_off] = MAX_BRIGHTNESS;
            else
                image[2][y + y_off][x + x_off] = (unsigned char)(MAX_BRIGHTNESS + da - damax);
            //                       image3[y+y_off][x+x_off] = (unsigned char)da;
        }

        for (x = x_off; x < width[2]; x++)
        {
            da = scale * log10(sqrt(data[y][x] * data[y][x] + jdata[y][x] * jdata[y][x]));
            if ((MAX_BRIGHTNESS + da - damax) < 0)
                image[2][y + y_off][x - x_off] = 0;
            //            else if ( da > MAX_BRIGHTNESS )
            //                     image3[y+y_off][x-x_off] = MAX_BRIGHTNESS;
            else
                image[2][y + y_off][x - x_off] = (unsigned char)(MAX_BRIGHTNESS + da - damax);
            //                     image3[y+y_off][x-x_off] = (unsigned char)da;
        }
    }

    for (y = y_off; y < height[2]; y++)
    {
        for (x = 0; x < width[2] / 2; x++)
        {
            da = scale * log10(sqrt(data[y][x] * data[y][x] + jdata[y][x] * jdata[y][x]));
            if ((MAX_BRIGHTNESS + da - damax) < 0)
                image[2][y - y_off][x + x_off] = 0;
            //            else if ( da > MAX_BRIGHTNESS )
            //                     image3[y-y_off][x+x_off] = MAX_BRIGHTNESS;
            else
                image[2][y - y_off][x + x_off] = (unsigned char)(MAX_BRIGHTNESS + da - damax);
            //                     image3[y-y_off][x+x_off] = (unsigned char)da;
        }

        for (x = x_off; x < width[2]; x++)
        {
            da = scale * log10(sqrt(data[y][x] * data[y][x] + jdata[y][x] * jdata[y][x]));
            if ((MAX_BRIGHTNESS + da - damax) < 0)
                image[2][y - y_off][x - x_off] = 0;
            //            else if ( da > MAX_BRIGHTNESS )
            //                     image3[y-y_off][x-x_off] = MAX_BRIGHTNESS;
            else
                image[2][y - y_off][x - x_off] = (unsigned char)(MAX_BRIGHTNESS + da - damax);
            //                     image3[y-y_off][x-x_off] = (unsigned char)da;
        }
    }

    ///////////////////////////////////////////////////////////////////////
}

void make_output_image(int n)
/* データ data[y][x], jdata[y][x] を画像No.nに代入 */
{
    int x, y;

    printf("\n逆FFT変換後の画像を作成します．\n");
    width[n] = num_of_data;
    height[n] = num_of_data;
    for (y = 0; y < height[n]; y++)
        for (x = 0; x < width[n]; x++)
        {
            if (data[x][y] < 0)
                data[x][y] = 0;
            if (data[x][y] > MAX_BRIGHTNESS)
                data[x][y] = MAX_BRIGHTNESS;
            image[n][x][y] = (unsigned char)data[x][y];
        }
}
