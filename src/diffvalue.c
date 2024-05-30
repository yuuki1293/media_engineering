/* diffvalue.c  このプログラムの名前 */
#include<stdio.h>
#include<math.h>
#include"pgmlib.h"
/* フィルタの係数（これはSobel）*/
int Cxij[3][3] =     /* Cij[j][i] の順になることに注意 */
   { { -1,  0,  1},  /* j=-1 で i=-1,i=0,i=+1 と変化 */
     { -2,  0,  2},  /* j= 0 で i=-1,i=0,i=+1 と変化 */
     { -1,  0,  1}}; /* j=+1 で i=-1,i=0,i=+1 と変化 */
int Cyij[3][3] =     /* Cij[j][i] の順になることに注意 */
   { { -1, -2, -1},  /* j=-1 で i=-1,i=0,i=+1 と変化 */
     {  0,  0,  0},  /* j= 0 で i=-1,i=0,i=+1 と変化 */
     {  1,  2,  1}}; /* j=+1 で i=-1,i=0,i=+1 と変化 */
int calc( int n, int x, int y );   /* 関数filteringの中で使用する関数 */
void filtering( int n1, int n2 );  /* フィルタリングを行う関数 */

int main(void)
{
    load_image( 0, "" );   /* ファイル → 画像No.0 */
    filtering( 0, 1 );     /* No.0->フィルタ->No.1 */  
    save_image( 1, "" );   /* 画像No.1 → ファイル */
    return 0;
}

int calc( int n, int x, int y )
/* 画像No.nに対して注目点(x,y)でフィルタリングしたときの */
/* 値を返す（近傍の画素は必ず画像内にあると想定している）*/
{
    int i,j;
    double dx=0.0,dy=0.0,d;

    for(j=0;j<3;j++){          /* 添え字は0から2 */
        for(i=0;i<3;i++){      /* 添え字は0から2 */
            /* i:x方向，j:y方向として，Cij[j][i]である */
            dx += Cxij[j][i] * image[n][x+i-1][y+j-1];
            dy += Cyij[j][i] * image[n][x+i-1][y+j-1];
        }
    }
    d = (int)(sqrt(dx*dx + dy*dy));
    return (int)d;
}

void filtering( int n1, int n2 )
/* 画像No.n1をフィルタリングして画像No.n2へ出力する */
{
    int x,y,xsft,ysft;
    int value, min=255, max=0; /* 計算結果の最小値と最大値 */

    /* 画像No.n2の画像サイズを入力 */
    width[n2]=width[n1];  height[n2]=height[n1];
    /* フィルタリングを行う（最小値・最大値を求める）*/
    /* 外周１画素は無視していることに注意してほしい  */
    for(y=1;y<height[n1]-1;y++){
        for(x=1;x<width[n1]-1;x++){
            value = calc( n1, x, y );
            if ( value < min ) min=value;
            if ( value > max ) max=value;
        }
    }
    printf("計算値の最大値＝%d, 最小値＝%d\n",max,min);
    /* フィルタリングを行う（未だ外周１画素は無視） */
    for(y=1;y<height[n1]-1;y++){
        for(x=1;x<width[n1]-1;x++){
            value = calc( n1, x, y );  /* 関数calcを利用 */
            image[n2][x][y] =
                (int)((double)(value - min)/(max - min)*255.0);
            if(x==100&&y==80)printf("変更前階調値: %d\nvalue: %d\n変更後階調値: %d\n", image[n1][x][y], value, image[n2][x][y]);
        }
    }
    /* 外周部の補間（座標を画像内にシフトして値をコピー） */
    for(y=0;y<height[n2];y++){
        for(x=0;x<width[n2];x++){
            xsft=0; ysft=0;  /* 内側の画素は両方0で良い */
            if ( x==0 ) xsft=1; else if ( x==width[n2]-1 )  xsft=-1;
            if ( y==0 ) ysft=1; else if ( y==height[n2]-1 ) ysft=-1;
            if ( xsft!=0 || ysft!=0 )  /* 外周部のみ処理 */
                image[n2][x][y] = image[n2][x+xsft][y+ysft];
        }
    }
}
