/* labeling.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
int label[MAX_IMAGESIZE][MAX_IMAGESIZE]; /* ラベル用 */
int inside( int n, int x, int y );
void labeling( int n1, int n2 );
void search( int n, int x, int y, int cnum );

int main(void)
{
    printf("２値画像（白：255，黒：0）を読み込みます\n");
    load_image( 0, "" );  /* ファイル → 画像No.0(原画像)   */
    labeling( 0, 1 );     /* ラベリング結果を画像No.1に作る */
    save_image( 1, "" );  /* 画像No.1 → ファイル */
    return 0;
}

int inside( int n, int x, int y ) /* 内外部判定 */
{
    if ( x >= 0 && x < width[n] && y >= 0 && y < height[n] )
    return 1;  else  return 0;
}

void search( int n, int x, int y, int cnum )
/* 画像No.nの座標(x,y)の周囲を探索．cnum:現在のラベル */
{
    if ( inside(n,x,y) && image[n][x][y]==0 && label[x][y]==0 ){
        label[x][y] = cnum;
        search( n, x, y-1, cnum ); /* 上の画素 */
        search( n, x-1, y, cnum ); /* 左の画素 */
        search( n, x, y+1, cnum ); /* 下の画素 */
        search( n, x+1, y, cnum ); /* 右の画素 */
    }
}

void labeling( int n1, int n2 )
/* 画像No.n1 中の黒い図形にラベリングを施して，結果を */
/* int label[ ][ ]に代入する．そして，ラベル値を階調  */
/* にした画像を画像No.n2に作る．ラベル番号が255を超え */
/* るときは 255 の剰余に変換している                  */
{
    int x,y;
    int count=0; /* 白い背景中の黒い孤立図形の総数 */

    /* ラベルと画像No.n2を初期化 */
    width[n2]=width[n1];  height[n2]=height[n1];
    for(y=0;y<height[n1];y++)
        for(x=0;x<width[n1];x++){
            label[x][y] = 0;  image[n2][x][y] = 0;
        }
    /* 画像No.n1をスキャン */
    for(y=0;y<height[n1];y++)
        for(x=0;x<width[n1];x++){
            if ( image[n1][x][y] == 0 && label[x][y] == 0 ){
                count++;  search( n1, x, y, count );
            }
        }
    /* 画像No.n2を作る */
    printf("孤立図形総数(ラベル最大値) = %d\n",count);
    if ( count > 0 )
        for(y=0;y<height[n2];y++)
            for(x=0;x<width[n2];x++)
                image[n2][x][y] = (label[x][y] * 50) % 255;
}
