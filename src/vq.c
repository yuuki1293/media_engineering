/* vq.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
#include<time.h>
#define NB   512  /* 最大ブロック数 */
#define NMAX  16  /* ブロックの一辺の画素数の最大 */
int rblock[NB][NMAX][NMAX];    /* 代表ブロック用 */
int codebook[NB][NB];    /* 各ブロックのコードブック */
int BXY,NX,NY; /* ブロックの一辺の画素数，横と縦の数 */
int NUM;     /* 代表ベクトルの総数 */

void clustering( int n1, int n2 );
int distance( int n1, int n2, int x, int y );
void refine_rblock( int n );
void input_data( int n );

int main(void)
{
    load_image( 0, "" );   /* ファイル --> 画像No.0   */
    input_data( 0 );       /* データの入力            */
    clustering( 0, 1 );    /* k平均法でクラスタリング */
    save_image( 1, "" );   /* 画像No.1 --> ファイル   */
    return 0;
}

void input_data( int n )
/* データの入力 */
{
    int work;
    
    do{
        printf("ブロックの一辺の画素数(<=%d):",NMAX);  scanf("%d",&BXY);
    }while( width[n] % BXY != 0 || height[n] % BXY != 0 || BXY > NMAX);
    NX = width[n]  / BXY;    NY = height[n] / BXY;
    printf("ブロック総数：%d×%d＝%d\n",NX,NY,NX*NY);
    if ( NB < NX*NY ) work=NB; else work=NX*NY;
    do{
        printf("代表ベクトルの本数(<=%d):",work);  scanf("%d",&NUM);
    }while( NUM<1 || NUM>NB || NUM>(NX*NY) );
}

void refine_rblock( int n )
/* 代表ベクトルを更新する */
{
    int i,x,y,s,t,counter;
    double work[NMAX][NMAX];
    
    for(i=0;i<NUM;i++){
        /* 代表ベクトル i で表現するブロックの個数を求める */
        counter=0;
        for(y=0;y<NY;y++)
            for(x=0;x<NX;x++)
                if (codebook[x][y]==i)  counter++;
        if (counter!=0){
            /* 作業領域を初期化 */
            for(t=0;t<BXY;t++)
                for(s=0;s<BXY;s++)
                    work[s][t] = 0.0;
            /* 重心の座標に更新する */
            for(y=0;y<NY;y++)
                for(x=0;x<NX;x++)
                    if (codebook[x][y]==i)
                        for(t=0;t<BXY;t++)
                            for(s=0;s<BXY;s++)
                                work[s][t] += image[n][BXY*x+s][BXY*y+t]/(double)counter;
            for(t=0;t<BXY;t++)
                for(s=0;s<BXY;s++)
                    rblock[i][s][t] = (int)work[s][t];
        }
    }
}

int distance( int n1, int n2, int x, int y )
/* 代表ベクトルNo.n1と，ブロック[x][y]との距離の２乗を返す */
/* 画像No.はn2 */
{
    int sum=0,s,t;
    
    for(t=0;t<BXY;t++)
        for(s=0;s<BXY;s++)
            sum = sum + (rblock[n1][s][t] - image[n2][BXY*x+s][BXY*y+t]) *
                        (rblock[n1][s][t] - image[n2][BXY*x+s][BXY*y+t]);
    return sum;
}

void clustering( int n1, int n2 )
/* ＶＱ法で画像No.n1-->No.n2に変換 */
{
    int i,j,k,x,y,s,t;
    unsigned char flag[NB][NB];  /* 選択フラグ */
    int change,min,minnum,dist,count=1;

    /* フラグの初期化 */
    for(j=0;j<NY;j++) for(i=0;i<NX;i++) flag[i][j]=0;
    /* 乱数で代表ベクトルを選択 */
    srand(time(NULL));
    for(i=0;i<NUM;i++){
        do{
            x = rand() % NX;  y = rand() % NY;
        }while( flag[x][y] != 0 );
        for(t=0;t<BXY;t++)
            for(s=0;s<BXY;s++)
                rblock[i][s][t] = image[n1][BXY*x+s][BXY*y+t];
        flag[x][y] = 1;
    }
    /* k-平均法によるクラスタリング */
    for(j=0;j<NY;j++)  for(i=0;i<NX;i++)  codebook[i][j] = -1;
    do{
        change = 0;
        printf("代表ベクトルの更新：%d回目\n",count);
        /* 各ブロックのコードブックを更新 */
        for(j=0;j<NY;j++){
            for(i=0;i<NX;i++){
                min = distance(0,n1,i,j);
                minnum = 0;
                for(k=1;k<NUM;k++){
                    dist = distance(k,n1,i,j);
                    if ( dist < min ){
                        min = dist;  minnum = k;
                    }
                }
                if ( minnum != codebook[i][j] ){
                    change = 1;  codebook[i][j] = minnum;
                }
            }
        }
        if ( change ) /* 代表ベクトルの更新 */
            refine_rblock( n1 );
        count++;
    }while( change==1 );  /* 変化がある限り実行 */
    /* 画像No.n2に圧縮画像を形成 */
    width[n2] = width[n1];  height[n2] = height[n1];
    for(y=0;y<NY;y++)
        for(x=0;x<NX;x++)
            for(t=0;t<BXY;t++)
                for(s=0;s<BXY;s++)
                    image[n2][BXY*x+s][BXY*y+t] = 
                        rblock[codebook[x][y]][s][t];
}
