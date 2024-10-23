/* curve1.c  このプログラムの名前 */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define PNTMAX 100000

/* Freeman の Chain code */
int c_code[8][2] = {
    {  1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 },
    { -1, 0 }, {-1,  1 }, { 0,  1 }, {  1,  1 }};
    
int pnt[PNTMAX][2];  /* 曲線の座標データ */
int pntnum=0;        /* 点の総数 */
void load_boundary(void);
void obtain_angles(void);
double angle( int num, int L );

int main(void)
{
    printf("輪郭線の元の画像を読み込みます\n");
    load_boundary( );  /* 輪郭線の読み込み */
    obtain_angles();   /* 偏角差分関数の計算と表示 */
    return 0;
}

double angle( int num, int L )
/* No.num の点の曲率を求めて計算する */
{
    int n1,n0,n2;
    double vx1,vy1,vx2,vy2,l1,l2;
    double value;

    n0 = num;  n1 = num - L;  n2 = num + L;
    if (n0<0) n0=pntnum+n0; else if (n0>pntnum-1) n0=n0-pntnum;
    if (n1<0) n1=pntnum+n0; else if (n1>pntnum-1) n1=n1-pntnum;
    if (n2<0) n2=pntnum+n0; else if (n2>pntnum-1) n2=n2-pntnum;
    vx1 = pnt[n0][0] - pnt[n1][0];
    vy1 = pnt[n0][1] - pnt[n1][1];
    vx2 = pnt[n2][0] - pnt[n0][0];
    vy2 = pnt[n2][1] - pnt[n0][1];
    l1 = sqrt( vx1 * vx1 + vy1 * vy1 );
    l2 = sqrt( vx2 * vx2 + vy2 * vy2 );
    if ( l1==0 || l2==0 ) value = 1.0;
    else {
        value = (vx1*vx2 + vy1*vy2)/l1/l2;
        if ( value >  1.0 ) value =  1.0; 
        else if ( value < -1.0 ) value = -1.0;
    }
    return acos( value );
}

void obtain_angles(void)
/* 偏角差分関数値を求める */
{
    int L; /* ベクトルの長さ */
    int i;

    printf("ベクトルの長さ：");
    scanf("%d",&L);
    for(i=L;i<pntnum+2*L;i++){  /* 少し多めにスキャン */
        printf("%3d, %2.3f\n",i,angle( i, L ) );
    }
}

void load_boundary(void)
/* 輪郭線データをpnt[ ][ ]に読み込む */
{
    FILE *fp;
    char str[256];  /* ファイル名用 */
    int x,y,c;

    /* 入力ファイルの指定 */
    printf("輪郭線のデータファイル名：");
    scanf("%s",str);
    if ( (fp=fopen(str,"r"))==NULL ) exit(1);
    fscanf( fp, "%d,%d",&pnt[pntnum][0], &pnt[pntnum][1] );
    do{
        fscanf( fp,"%d",&c );
        if ( c != -1 ){
            pntnum++;
            if ( pntnum < PNTMAX ){
                pnt[pntnum][0] = pnt[pntnum-1][0] + c_code[c][0];
                pnt[pntnum][1] = pnt[pntnum-1][1] + c_code[c][1];
            }
        }
    }while(c != -1);
    printf("点の総数＝%d\n",pntnum);
    if ( pntnum > PNTMAX ){
        pntnum = PNTMAX;
        printf("点の総数が想定値を超えたので，%dに変えました",PNTMAX);
    }
    fclose(fp);
}
