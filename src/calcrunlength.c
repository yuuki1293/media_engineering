/* calcrunlength.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
#define N 16
int bitmask[N];
void make_bitmask(int n);
void make_binary_image( int n, int value );
int make_runlength_code( int num );
void runlength_coding( int n );

int main(void)
{
    make_bitmask(N);    /* 1～Nビットまでのマスクを作る */
    printf("原画像を読み込みます．\n");
    load_image( 0, "" );          /* ファイル → 画像No.0 */
    make_binary_image( 0, 150 );  /* 固定しきい値120で２値化 */
    printf("　２値化画像を保存します．\n");
    save_image( 0, "" );          /* 画像No.0 → ファイル */
    runlength_coding( 0 );  /* 画像No.0をランレングス符号化 */
    return 0;
}

void  make_bitmask(int n)
/* 1～nビットまでのビットマスクを求める */
{
    int i;
    
    bitmask[0]=1;
    for(i=1;i<n;i++) bitmask[i] = bitmask[i-1] * 2;
}

void make_binary_image( int n, int value )
/* 画像No.n をしきい値value で２値化する */
{
    int x,y;

    for(y=0;y<height[n];y++){
        for(x=0;x<width[n];x++){
            if (image[n][x][y] < value) image[n][x][y] = 0;
            else image[n][x][y] = 255;
        }
    }
}

int make_runlength_code( int num )
/* 整数numをランレングス符号化する関数 */
{
    int n1=N,n2;

    num = num - 1;  /* まず1を引く */
    /* ランレングス部分のビット数 = n1 */
    if (num < 2) n1=2;
    else{
        do{
            n1--;
        }while( bitmask[n1]>num );
        n1++;
    }
    /* Wyle符号の割り当て */
    if (n1 == 2) n2=1;
    else n2 = n1 - 1;
    return n1+n2;
}

void runlength_coding( int n )
/* ２値画像No.nをランレングス符号化してデータ量を計算 */
{
    int x,y,bw,num;
    int linedata,totaldata=0;
    double total;

    printf("２値画像をランレングス符号化します．\n");
    for(y=0;y<height[n];y++){
        printf("line%4d:",y);
        linedata = 1; /* 最初の画素が白か黒かの情報 */
        bw = image[n][0][y];    /* 最初の画素の白黒 */
        num = 1;
        for(x=1;x<width[n];x++){
            if (image[n][x][y] == bw)
                num++;
            else {
                linedata += make_runlength_code(num);
                num = 0;
                bw = image[n][x][y];
            }
        }
        if (num != 0) linedata += make_runlength_code(num);
        printf("%5d [bit]\n",linedata);
        totaldata += linedata;  /* linedata は符号化せずに加えている */
    }
    printf("画像の総符号化データ量：%d [bit]\n",totaldata);
    total = width[n]*height[n]; /* [bit] */
    printf("データ圧縮率：%f\n",totaldata/total);
}
