/* dotpattern-r.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
#define CMAX 28
char abc[CMAX] = { 
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
  'u', 'v', 'w', 'x', 'y', 'z', ' ', '.' };
void return_white( int n, int *num1, int *num2 );
void extract_characters( int n );

int main(void)
{
    load_image( 0, "" );   /* ファイル → 画像No.0 */
    extract_characters( 0 );  /* 文字列の抽出 */
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

void extract_characters( int n )
/* No.n の透かし入り画像から文字列を抽出する */
{
    int x,y,s,t,i;
    int count,gray,dot[9],n1,n2,number;

    printf("混入されていた文字列：\n");
    for(y=0;y<height[n];y=y+3){
        for(x=0;x<width[n];x=x+3){
            /* ドットパターンに代入 */
            count=0;  gray=0;
            for(t=0;t<3;t++){
                for(s=0;s<3;s++){
                    dot[count] = image[n][x+s][y+t] / 255;
                    if ( dot[count] == 1 ) gray++;
                    count++;
                }
            }
            /* 階調２についてのみ処理する */
            if ( gray == 2 && (dot[0]!=1 || dot[1]!=1)){
                number=0;
                do{
                    number++;
                    return_white( number, &n1, &n2 );
                }while( dot[n1-1]!=1 || dot[n2-1]!=1 );
                printf("%c",abc[number-1]);
            }
        }
    }
    printf("\n");
}
