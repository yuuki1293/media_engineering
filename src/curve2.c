/* curve2.c  このプログラムの名前 */
#include<stdio.h>
#include<math.h>
#include"pgmlib.h"
#define PNTMAX 100000

/* Freeman の Chain code */
int c_code[8][2] = {
    {  1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 },
    { -1, 0 }, {-1,  1 }, { 0,  1 }, {  1,  1 }};
    
int pnt[PNTMAX][2];  /* 曲線の座標データ */
int pntnum=0;        /* 点の総数 */
double angles[PNTMAX];  /* 偏角差分関数値 */
void load_boundary(void);
void obtain_angles( int n );
double angle( int num, int L );
void line( int n, int x1, int y1, int x2, int y2, int col );

int main(void)
{
    printf("原画像のデータを読み込みます\n");
    load_image( 0, "" ); /* ファイル → 画像No.0 */
    printf("輪郭線の元の画像を読み込みます\n");
    load_boundary( );  /* 輪郭線の読み込み */
    obtain_angles( 0 );  /* 偏角差分関数の計算と表示 */
    save_image( 0, "" ); /* 画像No.0 → ファイル */
    return 0;
}

void line( int n, int x1, int y1, int x2, int y2, int col )
/* 画像No.nに(x1,y1),(x2,y2)を結ぶcol色の直線を描く */
{
    int i,num,w,h,x,y;
    double rate;
    
    w = abs(x2-x1);  h = abs(y2-y1);
    if (w>h) num=w*2; else num=h*2;
    for(i=0;i<=num;i++){
        rate = (double)i/num;
        x = (int)(x1 + rate*(x2-x1));
        y = (int)(y1 + rate*(y2-y1));
        image[n][x][y] = col;
    }
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

void obtain_angles( int n )
/* 偏角差分関数値を求める */
{
    int L; /* ベクトルの長さ */
    int i;
    double value;
    int count,flag,n1,n2,nm;
    int xy[PNTMAX][2];
    int x,y;

    printf("ベクトルの長さ：");
    scanf("%d",&L);
    for(i=L;i<pntnum+2*L;i++){  /* 少し多めにスキャン */
        angles[i] = angle( i, L );
    }
    /* 画像No.0 の黒画素の輝度を上げる */
    for(y=0;y<height[n];y++)
        for(x=0;x<width[n];x++)
            if (image[n][x][y]==0) image[n][x][y]=200;
    /* しきい値 */
    printf("しきい値：");  scanf("%lf",&value);
    count=0;  flag=0;
    for(i=L+2;i<pntnum+2*L-2;i++){
        if ( flag==0 && angles[i]>value ){
            n1=i;
            flag=1;
        } else if ( flag==1 && angles[i] < value ){
            n2=i-1;
            count++;
            nm = (n2 + n1)/2; 
            if ( nm < 0 ) nm = pntnum + nm; else
            if ( nm > pntnum-1 ) nm = nm-(pntnum-1);
            printf("%d:(%d, %d)\n",count,pnt[nm][0],pnt[nm][1]);
            xy[count-1][0]=pnt[nm][0];
            xy[count-1][1]=pnt[nm][1];
            flag=0;
        }
    }
    if ( count >= 2 ){
        count++;  xy[count-1][0] = xy[0][0];  xy[count-1][1] = xy[0][1];
        /* 直線の描画 */
        for(i=0;i<count-1;i++)
            line( 0, xy[i][0], xy[i][1], xy[i+1][0], xy[i+1][1], 0 );
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
