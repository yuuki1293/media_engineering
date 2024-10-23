/* boundary.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
int c_code[8][2] = {  /* Freeman の Chain code */
    {  1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 },
    { -1, 0 }, {-1,  1 }, { 0,  1 }, {  1,  1 }};
int next_code[8] = { 7, 7, 1, 1, 3, 3, 5, 5 };
void detect_boundary( int n );
int inside( int n, int x, int y );

int main(void)
{
    printf("２値画像（白：255，黒：0）を読み込みます\n");
    load_image( 0, "" );  /* ファイル → 画像No.0(原画像)  */
    detect_boundary( 0 ); /* 輪郭線を求めて保存する */
    return 0;
}

int inside( int n, int x, int y )
/* 内外部判定 */
{
    if ( x >= 0 && x < width[n] && y >= 0 && y < height[n] )
    return 1;  else  return 0;
}

void detect_boundary( int n )
/* 画像No.n中の黒い図形の輪郭線を求めて保存する */
{
    int x=0,y=0,xstart,ystart,xs,ys,xp,yp;
    FILE *fp;
    char str[256];  /* ファイル名用 */
    int c,cs,found=0,count=0;

    /* 出力ファイルの指定 */
    printf("求めた輪郭線を保存するときのファイル名：");
    scanf("%s",str);  if ( (fp=fopen(str,"w")) == NULL ) exit(1);
    /* 図形要素の輪郭線を１つだけ求めている */
    do{
        if ( image[n][x][y] == 0 ){  /* 黒画素を検出 */
            count=1; /* 輪郭線の画素総数 */
            xstart=x;  ystart=y;  /* 探索開始点 */
            fprintf( fp, "%d,%d\n",xstart,ystart);
            xs=xstart;  ys=ystart;  c=5; /* ＝初期探索方向 */
            do{
                found=0;  cs=c;
                do{
                    xp = xs + c_code[c][0];
                    yp = ys + c_code[c][1];
                    if ( inside(n,xp,yp) && image[n][xp][yp]==0 )
                        found=1;
                    else { c++;   if ( c > 7 ) c=0; }
                }while( found == 0 && c != cs );
                /* 次の画素を検出した場合，コードを出力 */
                if ( xp != xstart || yp != ystart ){
                    fprintf( fp, "%d\n", c );
                    count++; /* 総点数 */
                }
                xs = xp;  ys = yp;  /* 現在の点を移動 */
                c = next_code[c];   /* 次の探索方向を代入 */
            }while( (xp!=xstart || yp!=ystart) && found==1 );
        }
        x++;  if ( x == width[n] ){ x=0; y++; }
    }while( y < height[n] && found==0 );
    fprintf(fp,"%d\n",-1);  /* データの終わりに -1 を入れる */
    fclose(fp);
    if ( count > 0 ){
        printf("輪郭線の構成点数＝%d\n",count);
        printf("輪郭線は保存されました\n",str);
    } else printf("画像中に黒画素は検出されませんでした\n");
}
