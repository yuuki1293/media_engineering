/* areafilter.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
#include"labeling.h"
void filterling_using_area( int n );

int main(void)
{
    printf("２値画像（白：255，黒：0）を読み込みます\n");
    load_image( 0, "" );  /* ファイル → 画像No.0(原画像) */
    filterling_using_area( 0 );
    save_image( 0, "" );  /* 画像No.0 → ファイル */
    return 0;
}

void filterling_using_area( int n )
/* 画像No.n 中の黒い図形にラベリングを施した後， */
/* 面積でフィルタリングする                      */
{
    int i,x,y,number,area,max;

    number = labeling( n );  /* ← labeling.h の中の関数 */
    printf("孤立図形総数(ラベル最大値) = %d\n",number);
    if ( number > 0 ){
        /* 図形の面積を用いたフィルタリングを行う */
        printf("一定の面積以下の領域を削除します．\n");
        printf("削除する面積の最大値：");
        scanf("%d",&max);
        for(i=1;i<=number;i++){
            /* ラベル i の面積を求める */
            area=0;
            for(y=0;y<height[n];y++)
                for(x=0;x<width[n];x++)
                    if (label[x][y]==i) area++;
            /* area <= max のときは削除する */
            if ( area <= max )
                for(y=0;y<height[n];y++)
                    for(x=0;x<width[n];x++)
                        if (label[x][y]==i )
                            image[n][x][y] = 255;
        }
    }
}
