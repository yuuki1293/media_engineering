/* dilatation.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
void dilatation( int col, int n1, int n2 );
int count( int n, int x, int y, int col );

int main(void)
{
    int col;
    
    printf("２値画像（白：255，黒：0）を読み込みます\n");
    load_image( 0, "" ); /* ファイル → 画像No.0(原画像)  */
    printf("膨張させる色は？（黒：0，白：255）：");
    scanf("%d",&col);
    dilatation( col, 0, 1 );  /* No.0 に膨張処理を施してNo.1へ */
    save_image( 1, "" ); /* 画像No.1 → ファイル */
    return 0;
}

int count( int n, int x, int y, int col )
/* No.nの画像の座標(x,y)の8近傍中のcol色の画素数を返す */
{
    int s,t,xp,yp,counter=0;
    
    for(t=-1;t<2;t++)
        for(s=-1;s<2;s++){
            xp = x + s;  yp = y + t;
            if ( xp >= 0 && xp < width[n] 
              && yp >= 0 && yp < height[n]
              && image[n][xp][yp] == col )
                counter++;
        }
    return counter;
}

void dilatation( int col, int n1, int n2 )
/* ２値画像(No.n1)の階調colの図形を膨張させてNo.n2へ */
{
    int x,y,i,n,col1,col2;

    if (col==0) { col1=255;  col2=0; } else { col1=0;  col2=255; };
    /* 膨張させる回数を指定する */
    printf("膨張処理を行う回数：");  scanf("%d",&n);
    /* 膨張処理を施した画像をNo.n2に作る */
    copy_image( n1, n2 );  /* ← pgmlib.h 内の関数 */
    for(i=0;i<n;i++){
        /* 膨張処理後の画像をNo.n2へ */
        for(y=0;y<height[n1];y++)
            for(x=0;x<width[n1];x++)
                if ( image[n1][x][y] == col1 && count(n1,x,y,col2) > 0 )
                    image[n2][x][y] = col2;
        /* No.n2 を No.n1 へコピーする */
        copy_image( n2, n1 );  /* pgmlib.h 内の関数 */
    }
}
