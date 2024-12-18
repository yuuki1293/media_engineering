/* extract_motion1.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
void get_difference( int n1, int n2, int n3 );

int main(void)
{
    printf("●フレーム画像を読み込みます\n");
    load_image( 0, "" );        /* ファイル → 画像No.0 */
    printf("●背景画像を読み込みます\n");
    load_image( 1, "" );        /* ファイル → 画像No.1 */
    get_difference( 0, 1, 2 );  /* 差分画像 → 画像No.2 */
    return 0;
}

void get_difference( int n1, int n2, int n3 )
/* No.n1 と No.n2 の差分画像を No.n3 に代入する */
{
    int x,y,value;

    width[n3]=width[n1];  height[n3]=height[n1];
    for(y=0;y<height[n1];y++){
        for(x=0;x<width[n1];x++){
            image[n3][x][y] = abs( image[n1][x][y] - image[n2][x][y] );
        }
    }
    printf("●差分の絶対値を階調にした差分画像を保存します\n");
    save_image( n3, "" );
    printf("差分に対するしきい値：");
    scanf("%d",&value);
    for(y=0;y<height[n1];y++){
        for(x=0;x<width[n1];x++){
            if ( image[n3][x][y] > value )
                image[n3][x][y] = 0;
            else
                image[n3][x][y] = 255;
        }
    }
    printf("●差分がある部分を黒画素にした差分画像を保存します\n");
    save_image( n3, "" );

    for(y=0;y<height[n1];y++){
        for(x=0;x<width[n1];x++){
            if ( image[n3][x][y] == 255 )
                image[n1][x][y] = 200;
        }
    }

    save_image(n1, "");
}
