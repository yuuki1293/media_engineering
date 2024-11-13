/* shape_recog1.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
#include"labeling.h"  /* label[][], labeling()を含む */
void recognition( int n1, int n2 );

int main(void)
{
    printf("２値画像（白：255，黒：0）を読み込みます\n");
    load_image( 0, "" );   /* ファイル → 画像No.0(原画像)   */
    recognition( 0, 1 );   /* 画像No.0の認識結果を画像No.1へ */
    save_image( 1, "" );   /* 画像No.1 → ファイル */
    return 0;
}

void recognition( int n1, int n2 )
/* 画像No.n1を認識した結果の画像をNo.n2に作る */
{
    int i,num,x,y,h,w;
    int area,xmin,ymin,xmax,ymax,MINAREA;
    double ratio,MINRATIO;

    /* 画像No.n2 の準備 */  copy_image(n1,n2);
    /* ラベリング(labeling.hの関数labering()を利用) */
    num = labeling( n1 );
    printf("画像中の図形領域数＝%d\n",num);
    printf("面積のしきい値(例:1000)：");
    scanf("%d",&MINAREA);
    printf("外接四角形充填率のしきい値(例:0.5)：");
    scanf("%lf",&MINRATIO);
    /* 面積・外接四角形充填率で分類 */
    for(i=1;i<=num;i++){
        area=0;
        xmin=width[n1];   xmax=0;
        ymin=height[n1];  ymax=0;
        for(y=0;y<height[n1];y++)
            for(x=0;x<width[n1];x++)
                if (label[x][y] == i){
                    area++;
                    if (x < xmin) xmin=x;  if (x > xmax) xmax=x;
                    if (y < ymin) ymin=y;  if (y > ymax) ymax=y;
                }
        w = xmax - xmin;  if ( w == 0 ) w=1;
        h = ymax - ymin;  if ( h == 0 ) h=1;
        ratio = (double)area/w/h;
        for(y=0;y<height[n1];y++)
            for(x=0;x<width[n1];x++)
                if (label[x][y] == i){
                    if (ratio <= MINRATIO) image[n2][x][y]=160;
                    if (area  <= MINAREA) image[n2][x][y]=80;
                }
    }
}
