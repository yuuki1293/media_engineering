/* k-means.h  このプログラムの名前 */
#include<time.h>  /* for 乱数 */
#define MAXLABEL   500  /* 最大の孤立領域数 */
#define MAXDIM     5  /* 考慮する特徴ベクトルの最大次元数 */
#define MAXCLASS    10  /* 考慮する最大のk(クラス数) */
double fvector[MAXLABEL][MAXDIM];   /* 特徴ベクトル群 */
double rvector[MAXCLASS][MAXDIM];    /* 代表ベクトル   */
int codebook[MAXLABEL];  /* 各ベクトルのコードブック */

double _distance( int rn, int fn, int dim );
void k_means_clustering( int number, int dim, int classnum );

double _distance( int rn, int fn, int dim )
/* 代表ベクトルNo.rn と，ある1つの特徴ベクトル(No.fn)との */
/* 距離の２乗を返す関数．dim < MAXLABEL を前提とする． */
{
    double sum=0.0;
    int i;

    for(i=0;i<dim;i++)
    sum+=(rvector[rn][i]-fvector[fn][i])*(rvector[rn][i]-fvector[fn][i]);
    return sum;
}

void k_means_clustering( int number, int dim, int classnum )
/* k-means 法．number：ベクトル総数 < MAXLABEL */
/* dim：特徴ベクトルの次元数, classnum:クラス数(k-means の k) */
{
    int i,j,k,n,change,count=1,minnum;
    double min,dist,member;

    printf("number = %d\n",number);
    if (number > MAXLABEL){
        printf("領域数が想定最大値を超えています．\n");  exit(1);
    } else if ( dim < 1 || dim > MAXDIM ){
        printf("特徴の次元数(=dim)が不適切です\n");  exit(1);
    } else if ( classnum > MAXCLASS || classnum == 1 ){
        printf("クラス数(=k)が不適切です\n");  exit(1);
    } else {
        /* 初期代表ベクトルの決定（ランダムに選ぶ） */
        srand(time(NULL));
        for(i=0;i<number;i++) codebook[i] = -1;
        for(i=0;i<classnum;i++){
            do{  n = rand() % number;  }while(codebook[n] != -1);
            for(j=0;j<dim;j++) rvector[i][j] = fvector[n][j];
            codebook[n] = i;
        }
        /* k-平均法によるクラスタリング */
        do{
          change = 0;
          printf("代表ベクトルの更新：%d回目\n",count);
          /* 各ベクトルのコードブックを更新 */
          for(i=0;i<number;i++){  /* 各領域について調べる */
            min = _distance(0,i,dim); minnum = 0; /* 仮決め */
            for(j=1;j<classnum;j++){ /* 1～classnum-1までを調査 */
              dist = _distance(j,i,dim);
              if ( dist < min ){
                min = dist;  minnum = j;
              }
                } /* 領域iはクラスminnumに所属すると決定 */
                if ( codebook[i] != minnum ){
                    change = 1; /* コードブックが更新された */
                    codebook[i] = minnum;
                }
            }
            /* 代表ベクトルを更新 */
            if ( change ){
              for(i=0;i<classnum;i++){
                /* 各クラスのメンバー数の把握 */
                member=0;
                for(j=0;j<number;j++)
                  if (codebook[j]==i) member++;
                  /* 所属する特徴ベクトルの平均に置き換え */
                  for(j=0;j<dim;j++) rvector[i][j]=0.0;
                  for(k=0;k<number;k++)
                    if (codebook[k]==i)
                      for(j=0;j<dim;j++)
                        rvector[i][j] += fvector[k][j] / member;
             }
             count++;
          }
        }while( change );
    }
}
