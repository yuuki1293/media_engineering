/* make_bgd.c  このプログラムの名前 */
#include<stdio.h>
#include<stdlib.h>  /* for rw_serial.h */
#include<string.h>  /* for rw_serial.h */
#include"pgmlib.h"
#include"rw_serial.h"
double bgd[MAX_IMAGESIZE][MAX_IMAGESIZE];
void make_backgrounds( int n );

int main(void)
{
    make_backgrounds( 1 );     /* 画像No.1に背景画像を作成   */
    printf("\n背景画像を保存します\n");
    save_image( 1, "" );       /* 背景画像を保存 */
    return 0;
}

void make_backgrounds( int n )
/* 一連の画像列から背景画像を求めて画像No.nに代入する */
{
    int start,end,keta,w,h;
    char inh[128],outh[128],fname[256];
    int num,i,x,y;

    /* 連続処理のためのパラメータを入力する */
    set_serial_parameters(&start,&end,&keta,inh,outh,&w,&h);
    num = end - start + 1;  /* 処理する枚数 */
    /* 画像No.0に画像を読み込み，その平均をbgd[]に代入 */
    for(y=0;y<h;y++)  for(x=0;x<w;x++)  bgd[x][y] = 0.0;
    for(i=start;i<=end;i++){
        make_file_name( inh, i, keta, fname );
        printf("入力ファイル名＝%s\n",fname);
        load_image( 0, fname );
        for(y=0;y<h;y++)
            for(x=0;x<w;x++)
                bgd[x][y] += image[0][x][y] / (double)num;
    }
    /* 画像No.nに保存 */
    width[n]=w;  height[n]=h;
    for(y=0;y<h;y++)
        for(x=0;x<w;x++)
            image[n][x][y] = (unsigned char)( bgd[x][y] );
}
