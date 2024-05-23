/* median.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
void median( int n1, int n2 );

int main(void)
{
    load_image( 0, "" );   /* ファイル → 画像No.0 */
    median( 0, 1 );        /* No.0->フィルタ->No.1 */
    save_image( 1, "" );   /* 画像No.1 → ファイル */
    return 0;
}

void median( int n1, int n2 )
/* 画像No.n1をメディアンフィルタで処理して画像No.n2へ */
{
    int x,y,i,j,count,min,minnum;
    int neighbour[9],flag[9];
    int xsft,ysft;

    /* 画像No.n2の画像サイズを入力 */
    width[n2]=width[n1];  height[n2]=height[n1];
    init_image( n2, 0 );  /* 画像No.n2を0で初期化 */
    /* フィルタリング（外周１画素は無視） */
    for(y=1;y<height[n1]-1;y++){
        for(x=1;x<width[n1]-1;x++){
            /* 近傍の画素列を１次元配列 neighbour[] に代入 */
            count=0;
            for(j=-1;j<2;j++){
                for(i=-1;i<2;i++){
                    neighbour[count]=image[n1][x+i][y+j];
                    flag[count]=0;   /* =0:未処理，=1:選択済み を表す */
                    count++;
                }
            }
            /* 未処理の階調値列から最小値を見つける処理 */
            for(i=1;i<=5;i++){  /* ５回だけ行う */
                min = 255;  minnum = 0;  /* 仮の値 */
                for(j=0;j<9;j++){
                    if ( flag[j]==0 && neighbour[j]<=min ){
                        min = neighbour[j];
                        minnum = j;
                    }
                }
                flag[minnum]=1;  /* 最大値として選択済みを表す */
            }
            image[n2][x][y] = min;
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
