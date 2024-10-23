/* load_boundary.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
/* Freeman の Chain code */
int c_code[8][2] = {
    {  1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 },
    { -1, 0 }, {-1,  1 }, { 0,  1 }, {  1,  1 }};
void load_and_display_boundary( int n );

int main(void)
{
    printf("輪郭線の元の画像を読み込みます\n");
    load_image( 0, "" );  /* ファイル → 画像No.0(原画像)  */
    load_and_display_boundary( 0 ); /* 輪郭線の読み込みと表示 */
    save_image( 0, "" );  /* 画像No.0 → ファイル */
    return 0;
}

void load_and_display_boundary( int n )
/* 輪郭線データを読み込み，画像No.nに表示する */
{
    FILE *fp;
    char str[256];  /* ファイル名用 */
    int x,y,c;

    /* 原画像の黒画素の階調を下げる */
    for(y=0;y<height[n];y++){
        for(x=0;x<width[n];x++){
            if (image[n][x][y]==0) image[n][x][y]=200;
        }
    }
    /* 入力ファイルの指定 */
    printf("輪郭線のデータファイル名：");
    scanf("%s",str);
    if ( (fp=fopen(str,"r"))==NULL ) exit(1);
    fscanf( fp, "%d,%d",&x, &y );
    image[n][x][y] = 0;  /* 輪郭線の画素を階調0にする */
    do{
        fscanf( fp,"%d",&c );
        if ( c != -1 ){
            x = x + c_code[c][0];  y = y + c_code[c][1];
            image[n][x][y] = 0;
        }
    }while(c != -1);
    fclose(fp);
}
