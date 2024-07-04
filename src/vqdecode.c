/* vqdecode.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
#include<time.h>
#define NB   512  /* 最大ブロック数 */
#define NMAX  16  /* ブロックの一辺の画素数の最大 */
int rblock[NB][NMAX][NMAX];    /* 代表ブロック用 */
int codebook[NB][NB];    /* 各ブロックのコードブック */
int BXY,NX,NY; /* ブロックの一辺の画素数，横と縦の数 */
int NUM;     /* 代表ベクトルの総数 */

void read_data(void);
void make_image( int n );

int main(void)
{
    read_data();          /* 出力データの読み込み */
    make_image( 0 );      /* 複号化画像の作成     */
    save_image( 0, "" );  /* 画像No.0 → ファイル */
    return 0;
}

void make_image( int n )
/* 復号化画像を画像No.nに作る */
{
    int i,j,x,y,number;

    width[n] = BXY * NX;  height[n] = BXY * NY;
    for(y=0;y<NY;y++){
        for(x=0;x<NX;x++){
            number = codebook[x][y];
            for(j=0;j<BXY;j++){
                for(i=0;i<BXY;i++){
                    image[n][BXY*x+i][BXY*y+j] = rblock[number][i][j];
                }
            }
        }
    }
}

void read_data(void)
/* vqcode.c による出力データの読み込み */
{
    int i,j,x,y;
    FILE *fp;
    char fname[256];

    printf("vqcode.c によって保存されたデータを読み込みます．\n");
    printf("入力ファイル名：");
    scanf("%s",fname);
    if ((fp=fopen(fname,"r"))==NULL){
        printf("ファイルがオープンできません．\n");
        exit(1);
    }
    fscanf(fp, "%d",&BXY);  /* ブロックサイズ     */
    printf("　ブロックサイズ：%d\n",BXY);
    fscanf(fp, "%d",&NUM);  /* 代表ベクトルの本数 */
    printf("　代表ベクトルの本数：%d\n",NUM);
    /* 代表ベクトルのデータの読み込み */
    for(i=0;i<NUM;i++){
        for(y=0;y<BXY;y++){
            for(x=0;x<BXY;x++){
                fscanf(fp, "%d,",&rblock[i][x][y]);
                printf("%d",rblock[i][x][y]);
                if (x!=BXY-1) printf(",");
            }
            printf("\n");
        }
    }
    /* ルックアップテーブルの読み込み */
    fscanf(fp,"%d,%d",&NX,&NY);
    printf("　横ブロック数：%d，縦ブロック数：%d\n",NX,NY);
    for(j=0;j<NY;j++){
        for(i=0;i<NX;i++){
            fscanf(fp,"%d",&codebook[i][j]);
            printf("%d ",codebook[i][j]);
        }
        printf("\n");
    }
    fclose(fp);
    printf("データは正しく読み込まれました．\n");
}
