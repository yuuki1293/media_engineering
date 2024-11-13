/* shape_recog2.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
void matching( int n1, int n2, int n3 );

int main(void)
{
    printf("●原画像を読み込みます\n");
    load_image( 0, "" );   /* ファイル → 画像No.0(原画像) */
    printf("●文字のテンプレート画像を読み込みます\n");
    load_image( 1, "" );   /* ファイル → 画像No.1(テンプレート) */
    /* 画像No.0のマッチング部分を反転．No.2：出力画像 */
    matching( 0, 1, 2 );
    save_image( 2, "" );   /* 画像No.1 → ファイル */
    return 0;
}

void matching( int n1, int n2, int n3 )
/* 原画像(n1)上をテンプレート(n2)で走査し一致部分を反転 */
/* (n3)は出力画像 */
{
    int x,y,s,t,sum,mindif;

    /* 画像No.n3 の準備 */  copy_image(n1,n3);
    /* マッチング処理 */
    mindif = width[n2]*height[n2];
    for(y=0;y<height[n1]-height[n2];y++){
        if (y % 10 == 0) printf("line%d\n",y);
        for(x=0;x<width[n1]-width[n2];x++){
            sum=0;
            for(t=0;t<height[n2];t++)
                for(s=0;s<width[n2];s++)
                    sum += abs( image[n1][x+s][y+t] - image[n2][s][t] );
            if ( sum < mindif )
                for(t=0;t<height[n2];t++)
                    for(s=0;s<width[n2];s++)
                        image[n3][x+s][y+t] = 255 - image[n1][x+s][y+t];
        }
    }
}
