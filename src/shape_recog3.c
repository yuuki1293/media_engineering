/* shape_recog3.c  このプログラムの名前 */
#include<stdio.h>
#include"pgmlib.h"
#include"labeling.h"  /* label[][], labeling()を含む */
#include"k-means.h"   /* k_means_clustering()を含む */
void clustering( int n1, int n2 );

int main(void)
{
    printf("２値画像（白：255，黒：0）を読み込みます\n");
    load_image( 0, "" );  /* ファイル → 画像No.0(原画像)   */
    clustering( 0, 1 );   /* 画像No.0の分類結果を画像No.1へ */
    save_image( 1, "" );  /* 画像No.1 → ファイル */
    return 0;
}

void clustering( int n1, int n2 )
/* 画像No.n1をクラスタリングした結果の画像をNo.n2に作る */
{
    int i,num,x,y,w,h,dim,classnum,gray;
    int area,xmin,ymin,xmax,ymax;

    /* 画像No.n2 の準備 */
    copy_image(n1,n2);
    /* ラベリング(labeling.hの関数labering()を利用) */
    num = labeling( n1 );
    printf("画像中の図形領域数＝%d\n",num);
    /* 面積・外接四角形充填率を求める */
    dim = 2;  /* ＝特徴ベクトルの次元数が２ */
              /* 最大次元数：MAXDIM in k-means.h */
    if (num > MAXLABEL) num=MAXLABEL; /* k-means.h参照 */
    for(i=1;i<=num;i++){
        area=0;
        xmin=width[n1];   xmax=0;
        ymin=height[n1];  ymax=0;
        for(y=0;y<height[n1];y++)
            for(x=0;x<width[n1];x++)
                if (label[x][y] == i){
                    area++;  /* 面積計算 */
                    /* 外接四角形の更新 */
                    if (x < xmin) xmin=x;  if (x > xmax) xmax=x;
                    if (y < ymin) ymin=y;  if (y > ymax) ymax=y;
                }
        w = xmax - xmin;  if (w==0) w=1;
        h = ymax - ymin;  if (h==0) h=1;
        /* 特徴ベクトル fvector[][](in k-means.h)へ代入 */
        fvector[i-1][0] = (double)area; /* 面積 */
        fvector[i-1][1] = (double)area / h / w; /* 外接四角形充填率 */
    }
    /* k-平均法でクラスタリングする */
    /* 最大クラス数(kの最大値)：MAXCLASS in k-means.h */
    printf("k-平均法でクラスタリング\n");
    do{
        printf("　クラスの総数(=k)[2,%d]：",MAXCLASS);
        scanf("%d",&classnum);
    }while(classnum<2 || classnum>MAXCLASS);
    k_means_clustering( num, dim, classnum );
    /* codebook[](in k-means.h)を使って階調を決定 */
    for(i=1;i<=num;i++){
        gray = codebook[i-1]  * ( 255 / classnum );
        for(y=0;y<height[n1];y++)
            for(x=0;x<width[n1];x++)
                if (image[n2][x][y]==0 && label[x][y]==i)
                    image[n2][x][y] = gray;
    }
}
