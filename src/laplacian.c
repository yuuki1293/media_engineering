/* laplacian.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"

/* フィルタの係数（これは８近傍ラプラシアン）*/
int Cij[3][3] =      /* Cij[j][i] の順になることに注意 */
   {{ 1,  1,  1},    /* j=-1 で i=-1,i=0,i=+1 と変化 */
    { 1, -8,  1},    /* j= 0 で i=-1,i=0,i=+1 と変化 */
    { 1,  1,  1}};   /* j=+1 で i=-1,i=0,i=+1 と変化 */
double K = 1.0; /* 最後に掛ける係数 */

int calc( int n, int x, int y );
void filtering( int n1, int n2 );

int main(void)
{
    load_image( 0, "" );   /* ファイル → 画像No.0 */
    filtering( 0, 1 );     /* No.0->フィルタ->No.1 */  
    save_image( 1, "" );   /* 画像No.1 → ファイル */
    return 0;
}

int calc( int n, int x, int y )
/* 画像No.nにフィルタリングした値を計算して返す */
/* 近傍の画素は必ず画像内にあると想定している   */
{
    int i,j,value=0;

    for(j=0;j<3;j++){     /* 添え字は0から2 */
        for(i=0;i<3;i++){ /* 添え字は0から2 */
            /* i:x方向，j:y方向として，Cij[j][i]である */
            value += Cij[j][i] * image[n][x+i-1][y+j-1];
        }
    }
    return (int)(K * value);
}

void filtering( int n1, int n2 )
/* 画像No.n1をフィルタリングして画像No.n2へ */
{
    int x,y,xsft,ysft;
    int value, min=255, max=0; /* 計算値の最小値と最大値 */

    /* 画像No.n2の画像サイズを入力 */
    width[n2]=width[n1];  height[n2]=height[n1];
    /* フィルタリングを行う（最小値・最大値を求める）*/
    /* 外周１画素は無視している */
    for(y=1;y<height[n1]-1;y++){
        for(x=1;x<width[n1]-1;x++){
            value = calc( n1, x, y );
            if ( value < min ) min=value;
            if ( value > max ) max=value;
        }
    }
    printf("計算値の最大値＝%d, 最小値＝%d\n",max,min);
    /* フィルタリングを行う（外周１画素は無視） */
    for(y=1;y<height[n1]-1;y++){
        for(x=1;x<width[n1]-1;x++){
            value = calc( n1, x, y );
            image[n2][x][y] = (int)((double)(value - min)/(max - min)*255.0);
        }
    }
    /* 外周部の補間 */
    for(y=0;y<height[n2];y++){
        for(x=0;x<width[n2];x++){
            xsft=0; ysft=0;
            if ( x==0 ) xsft=1; else if ( x==width[n2]-1 )  xsft=-1;
            if ( y==0 ) ysft=1; else if ( y==height[n2]-1 ) ysft=-1;
            if ( xsft!=0 || ysft!=0 )
                image[n2][x][y] = image[n2][x+xsft][y+ysft];
        }
    }
}
