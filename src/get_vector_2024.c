/* get_vector.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
#define WIN_W  5  /* ウィンドウ横：WIN_W*2+1 */
#define WIN_H  5  /* ウィンドウ縦：WIN_H*2+1 */
#define XSCAN 10  /* 走査範囲：XSCAN*2+1     */
#define YSCAN 10  /* 走査範囲：YSCAN*2+1     */
#define STEP   10  /* 速度を求める間隔[画素]  */
unsigned char window[WIN_W*2+1][WIN_H*2+1]; /* テンプレート用 */

int inside( int n, int x, int y );
void calc_vector( int n1, int n2, int n3 );
void draw_line( int n, int x1, int y1, int x2, int y2, int col );

int main(void)
{
    printf("●第１フレームを読み込みます\n");
    load_image( 0, "" );      /* ファイル → 画像No.0 */
    printf("●第２フレームを読み込みます\n");
    load_image( 1, "" );      /* ファイル → 画像No.1 */
    calc_vector( 0, 1, 2 );   /* 速度画像を画像No.2に作る  */
    save_image( 2, "" );      /* 速度画像を保存 */
    return 0;
}

void draw_line( int n, int x1, int y1, int x2, int y2, int col )
/* 画像No.nに(x1,y1),(x2,y2)を端点とする階調colの線分を描く */
{
    int num,i,x,y;
    
    if ( abs(x2-x1)>abs(y2-y1) ) num=2*abs(x2-x1);
    else num=2*abs(y2-y1);
    if (num==0) image[n][x1][y1] = col;
    else{
        for(i=0;i<=num;i++){
            x = x1 + (int)( i * (x2-x1)/(double)num );
            y = y1 + (int)( i * (y2-y1)/(double)num );
            image[n][x][y] = col;
        }
    }
}

int inside( int n, int x, int y )
/* ウィンドウを設定したとき内部に収まるとき１を返す */
{
    if ( x - WIN_W >= 0 && x + WIN_W < width[n] && 
         y - WIN_H >= 0 && y + WIN_H < height[n] )
        return 1;  else return 0;
}

void calc_vector( int n1, int n2, int n3 )
/* 第１フレーム(n1)と第２フレーム(n2)から速度画像(n3)を作る */
{
    int x1,y1,s,t,i,j;
    int x2,y2,sum,min,xs,ys;
    double sum_x, sum_y, count;

    printf("●速度ベクトルを求めます\n");
    /* 速度画像を初期化(原画像を暗くした画像にする) */
    copy_image(n1,n3);
    for(y1=0;y1<height[n3];y1++)
        for(x1=0;x1<width[n3];x1++)
            image[n3][x1][y1] = image[n3][x1][y1] / 2;
    /* 第１フレーム画像をスキャンする */
    for(y1=WIN_H;y1<height[n1]-WIN_H;y1=y1+STEP){
        if (y1 % 10 == 0) printf("line %d\n",y1);
        for(x1=WIN_W;x1<width[n1]-WIN_W;x1=x1+STEP){
            /* 第１フレーム画像でウィンドウを作る */
            for(t=0;t<2*WIN_H+1;t++)
                for(s=0;s<2*WIN_W+1;s++)
                    window[s][t]=image[n1][x1+s-WIN_W][y1+t-WIN_H];
            /* 第２フレーム画像上でスキャンする */
            min = (WIN_W*2+1)*(WIN_H*2+1)*255; /* 最大値に仮設定 */
            xs=0;  ys=0;   /* 終点の初期化 */
            for(j=-YSCAN;j<=YSCAN;j++)
                for(i=-XSCAN;i<=XSCAN;i++){
                    x2=x1+i;  y2=y1+j;
                    /* (x2, y2）を中心にするウィンドウが画像内なら */
                    if ( inside(n2, x2-WIN_W, y2-WIN_H) &&
                        inside(n2, x2+WIN_W, y2+WIN_H) ){
                        sum=0;
                        for(t=0;t<2*WIN_H+1;t++)
                            for(s=0;s<2*WIN_W+1;s++)
                                sum += abs(window[s][t] - 
                                    image[n2][x2+s-WIN_W][y2+t-WIN_H]);
                        if (sum < min){
                            min = sum;
                            xs = i;  ys = j;
                        } else if ( sum == min )
                            if ( abs(i)+abs(j) < abs(xs)+abs(ys) ){
                                xs = i;  ys = j;
                            }
                    }
                }
            /* 画像No.n3にベクトルの線分を描く */
            draw_line( n3, x1, y1, x1+xs, y1+ys, 255 );
            sum_x += xs;
            sum_y += ys;
            count ++;
        }
    }

    printf("%lf %lf\n", sum_x / count, sum_y / count);
}
