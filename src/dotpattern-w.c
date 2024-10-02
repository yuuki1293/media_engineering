/* dotpattern-w.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
#define CMAX 28
char abc[CMAX] = { 
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
  'u', 'v', 'w', 'x', 'y', 'z', ' ', '.' };
void return_white( int n, int *num1, int *num2 );
void hide_characters( int n1, int n2, char str[] );

int main(void)
{
    load_image( 0, "" );   /* ファイル → 画像No.0 */
    /* 原画像No.0をドットパターン化してNo.1に代入し，*/
    /* 文字列"this image was made by taro."を混入．*/
    hide_characters( 0, 1, "this image was made by jyouhou." );
    save_image( 1, "" );   /* 画像No.1 → ファイル */
    return 0;
}

void return_white( int n, int *num1, int *num2 )
/* n=1～28 を指定すると，白画素にする画素２つの */
/* ラスタ走査順での順番（1～9）を返す関数       */
{
    if ( n <=  7 ) { *num1=1; *num2=n+2;  } else
    if ( n <= 14 ) { *num1=2; *num2=n-5;  } else
    if ( n <= 20 ) { *num1=3; *num2=n-11; } else
    if ( n <= 25 ) { *num1=4; *num2=n-16; } else
                   { *num1=5; *num2=n-20; }
}

void hide_characters( int n1, int n2, char str[] )
/* No.n1の階調画像を，3×3画素のドットパターンで２値 */
/* 画像（擬似階調数=10）に直した後，階調２の部分に文 */
/* 字列 str[ ] をドットパターンに変更して混入する．  */
{
    int x,y,s,t,i;  /* 制御変数 */
    int cnt1,cnt2=0,cnt3=0;  /* cnt2:階調2の画素数 */
    char *pnt;  /* 文字列走査用ポインタ */
    int num1,num2; /* 白画素にするビットの番号 */
    
    /* 3×3ドットパターンの画像をNo.n2に形成 */
    width[n2]  = 3 * width[n1];   /* No.n2はn1の３倍 */
    height[n2] = 3 * height[n1];  /* No.n2はn1の３倍 */
    for(y=0;y<height[n1];y++){
        for(x=0;x<width[n1];x++){
            /* 10階調[0,9]にする */
            image[n1][x][y] = 
              (unsigned char)(image[n1][x][y] / 256.0 * 10 );
            /* 階調2の画素数をカウント */
            if ( image[n1][x][y] == 2 ) cnt2++;
            /* 3x3ドットパターン画像をNo.n2に形成 */
            cnt1=0;
            for(t=0;t<3;t++){
                for(s=0;s<3;s++){
                    if ( image[n1][x][y] > cnt1 ){
                        image[n2][x*3+s][y*3+t] = 255;
                        cnt1++;
                    } else image[n2][x*3+s][y*3+t] = 0;
                }
            }
        }
    }
    printf("この画像には全部で%dビット混入可能です\n",cnt2);
    /* 透かし情報の埋め込み */
    pnt = str;  /* ポインタが文字配列の先頭を指すようにした */
    while( *(pnt) != '\0' ){
        pnt++;  cnt3++;
    }
    printf("%dビットの文字列を混入します\n",cnt3);
    if ( cnt3 > cnt2 ){
        /* エラー処理 */
        printf("原画像を別の画像に変えて下さい\n");
        exit(1);
    } else {
        printf("文字列データを画像に混入します\n");
        /* 組み込み処理 */
        pnt = str;
        for(y=0;y<height[n1];y++){
            for(x=0;x<width[n1];x++){
                if ( image[n1][x][y]==2 ){
                    if ( *(pnt) != '\0' ){
                        i=-1;
                        do{
                            i++;
                        }while( (abc[i] != *(pnt)) && i<CMAX );
                        /* 組み込む文字は abc[i] */
                        /* まずは3x3をすべて黒画素に戻す */
                        for(t=0;t<3;t++)
                            for(s=0;s<3;s++)
                                image[n2][x*3+s][y*3+t] = 0;
                        /* 文字に応じた2画素を白画素に */
                        return_white( i+1, &num1, &num2 );
                        cnt1=0;
                        for(t=0;t<3;t++){
                            for(s=0;s<3;s++){
                                cnt1++;
                                if ( cnt1==num1 || cnt1==num2 )
                                    image[n2][x*3+s][y*3+t] = 255;
                            }
                        }
                        pnt++;  /* 走査ポインタを進める */
                    }
                }
            }
        }
    }
}