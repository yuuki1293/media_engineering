/* areafilter.c  このプログラムの名前 */
#include<stdio.h>
#include<wchar.h>
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
    int i,x,y,number,area,max,x0,y0;
    //                      1,  2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,   22,   23,   24
    wchar_t trans[] = {u"n", u"1", "2", "3", "4", "6", "8", "9", "0", "5", "7", "f", "b", "d", "a", "c", "e", u"g", u"B", u"C", u"A", u"D", u"ウ", u"イ", u"ア"};
    char trans[] = " 1234689057fbdacegBCADウイア";

    number = labeling( n );  /* ← labeling.h の中の関数 */
    printf("孤立図形総数(ラベル最大値) = %d\n",number);
    if ( number > 0 ){
        /* 図形の面積を用いたフィルタリングを行う */
        // printf("一定の面積以下の領域を削除します．\n");
        // printf("削除する面積の最大値：");
        // scanf("%d",&max);
        for(i=1;i<=number;i++){
            /* ラベル i の面積を求める */
            area=0;
            x0 = y0 = 0;
            for(y=0;y<height[n];y++)
                for(x=0;x<width[n];x++)
                    if (label[x][y]==i) {
                        area++;
                        if(x0 == 0){
                            x0 = x;
                            y0 = y;
                        }
                    }
            putwchar(trans[i]);
            printf(" label = %d area = %d x = %d y = %d \n", i, area, x0, y0);
        }
    }
}
