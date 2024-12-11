/* K-平均法による点のクラスタリングのプログラム Kmeans.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"pgmlib.h"

#define THRESH 180

#define PI  3.141592653589  /* 円周率 */
#define MAX_THETA   360  /* θ軸のサイズ（0.5[deg]=１画素）*/
#define MAX_RHO     720  /* ρ軸のサイズ                 */
#define MAX_PNTS   3000     /* 想定する最大構成点数    */
#define MAX_CLSTR  3000     /* 想定する最大クラスタ数  */
#define DARK_PIXEL  128     /* 暗い画素の階調値 */

int points[MAX_PNTS][2];        /* 点の座標                */
int num_pnts;                   /* 点の総数                */
int point_clstr[MAX_PNTS];      /* 点が属するクラスタ番号  */

int center_clstr[MAX_CLSTR][2]; /* クラスタの代表点の座標  */
int num_clstr;                  /* クラスタの総数          */

void swap_int( int *n1, int *n2 )
/* int n1 と int n2 を入れ替える */
{
    int n;  /* 作業変数 */

    n = *n1;    *n1 = *n2;    *n2 = n;
}

void extract_line(int img, int xs, int ys )
/* θρ平面上（image1上）の点(xs,ys)を画像上の直線に逆変換し，   */
/* 結果を image2[y][x] に描く                                    */
{
    double theta, rho, rho_max, _sin, _cos;  /* 作業変数 */
    int x, y;  /* 制御変数 */

    theta = 180.0 * xs / MAX_THETA;  /* [deg] */
    _sin = sin( theta / 180.0 * PI );
    _cos = cos( theta / 180.0 * PI );
    rho_max = 1.0 / 1.5 * width[img] + 1.0 / 1.5 * height[img];
    rho = ( ys - MAX_RHO / 2.0 ) * rho_max / MAX_RHO * 2.0; 
    /*  rho = x _cos + y _sin  --->  y = rho/_sin - x _cos/_sin  */
    if ( 45.0 <= theta && theta <= 135.0 ){ /* 数値の単位は[deg] */
        for ( x = 0; x < width[img]; x ++ ){
            y = (int)( ( rho - x * _cos ) / _sin );
            if ( 0 <= y && y < height[2] )
                image[img][x][y] = MAX_BRIGHTNESS; 
        }
    } else {
        for ( y = 0; y < height[img]; y ++ ){
            x = (int)( ( rho - y * _sin ) / _cos );
            if ( 0 <= x && x < width[2] )
                image[img][x][y] = MAX_BRIGHTNESS;
        }
    }
    printf("theta = %f rho = %f\n", theta, rho);
}

void inverse_Hough_transform(int sp, int img)
/* image1 を θρ平面とみなし，しきい値より上の点を画像空間に */
/* 逆変換して画像中に直線を描く．結果は image2 に代入する．   */
{
    int threshold,counter;  /* しきい値，直線のカウンタ */
    int x,y;  /* 制御変数 */
    char str[10];  /* 作業変数 */

    printf("読み込んだθρ平面から，画像上の直線を再現します．\n");
    do {
        printf("θρ平面上の累積に対するしきい値 (0～255の値）= ");
        scanf("%d",&threshold);
        counter = 0;
        for ( y = 0; y < MAX_RHO;  y ++ )
            for ( x = 0; x < MAX_THETA; x ++ )
                if ( image[sp][x][y] >= threshold ) counter ++;
        printf("全部で %d 本の直線が検出されました．\n", counter);
        printf("この値で OK ? ( y / n ) :");
        scanf("%s",str);
    } while ( str[0] != 'y' && str[0] != 'Y' );
    /* 逆変換処理の実行 */
    for ( y = 0; y < MAX_RHO; y ++ )
        for ( x = 0; x < MAX_THETA; x ++ )
            if ( image[sp][x][y] >= threshold )
                extract_line(img, x, y );
}

void move_image1_to_image2( )
/* 原画像データをimage2[y][x]に退避する */
/* その際，階調を落とす (=80)          */
{
    int x, y;  /* 制御変数 */

    width[2] = width[1];    height[2] = height[1];
    for ( y = 0; y < height[1]; y ++ )
        for ( x = 0; x < width[1]; x ++ ){
            image[2][x][y] = image[1][x][y];
            if ( image[2][x][y] == MAX_BRIGHTNESS )
                image[2][x][y] = 80;
        }
}

void check_data(int img)
{
    if ( width[img] != MAX_THETA || height[img] != MAX_RHO ){
        printf("これは対象とするθρ平面のデータと異なります．\n");
        exit(1);
    }
}

void draw_a_straight_line(int img, int x1, int y1, int x2, int y2 )
/* image2[y][x] 上の２点(x1,y1)，(x2,y2)間の直線上の配列要素 */
/* の値をインクリメントする． */
{
    double distance, step, t;   /* t : 内分点発生用制御変数 */
    int x, y;  /* 描く点の座標 */

    distance = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ); /* 2点間の距離 */
    step = 1.0 / ( 1.5 * distance );  /* 発生させる内分点の総数 */
    t = 0.0;
    while ( t < 1.0 ){
        x = (int)( t * x2 + ( 1.0 - t ) * x1 );
        y = (int)( t * y2 + ( 1.0 - t ) * y1 );
        if ( x >= 0 && x < width[img] && y >= 0 && y < height[img]
            && image[img][x][y] < MAX_BRIGHTNESS ){
            image[img][x][y] ++;
        }
        t = t + step;
    }
}

int _calc_rho( double px, double py, double r_mg, int theta )
/* ρを計算するルーチン（draw_a_curveで使われている） */
{
    return (int)(r_mg * ( px * cos( theta * PI / MAX_THETA ) 
               + py * sin( theta * PI / MAX_THETA ) ) + MAX_RHO / 2.0);
}

void draw_a_curve(int src, int dst, double px, double py )
/* ρ＝ｘ・cosθ＋y・sinθの曲線を，配列image2[rho][theta]に描く．   */
{
    double rho_max;  /* ρ軸方向の最大値[画素] */
    double rho_c;    /* ρ方向の係数 */
    int theta;       /* θの単位は[deg]  */

    rho_max = 1.0 / 1.5 * width[src] + 1.0 / 1.5 * height[src];
    rho_c = MAX_RHO / 2.0 / rho_max;
        /* データから座標への変換係数 */
    for ( theta = 0; theta < MAX_THETA; theta ++ ){
        draw_a_straight_line(dst, theta, _calc_rho( px, py, rho_c, theta),
            theta + 1, _calc_rho( px, py, rho_c, theta + 1) );
    }
}

void Hough_transform(int src, int dst )
/* 原画像 image1[y][x] を Hough 変換し，結果をθρ平面 */
/* image2[rho][theta] に代入する．ただしθρ平面の横軸・縦軸の */
/* 大きさは MAX_THETA, MAX_RHO であらかじめ決められている．*/
/* また，MAX_BRIGHTNESS(=255)以上の累積は考慮しない． */
{
    int x, y;

    /* θρ平面の初期化 */
    width[dst] = MAX_THETA;    height[dst] = MAX_RHO;
    for ( y = 0; y < height[dst]; y ++ )
        for ( x = 0; x < width[dst]; x ++ )
            image[dst][x][y] = 0;
    /* 変換処理の実行 */
    printf("Hough 変換を実行中です．\n");
    for ( y = 0; y < height[src]; y ++ ){
        for ( x = 0; x < width[src]; x ++ ){
            if ( image[src][x][y] == MAX_BRIGHTNESS ){
                draw_a_curve(src, dst, (double)x, (double)y );
            }
        }
    }
}

void draw_a_circle(int img, int x, int y, int r )
/* image2[y][x] に中心 (x,y) 半径 r の円を描く．全周で最大   */
/* 360 個の点を描くが，大きい円の場合は点線になることもある．*/
{
    int theta;     /* 角度 [deg] の制御変数     */
    int xp, yp;    /* 描画する点 (xp,yp) の座標 */

    for ( theta = 0; theta < 360; theta ++ ){
        xp = (int)( x + r * cos( theta * PI / 180.0 ) );
        yp = (int)( y + r * sin( theta * PI / 180.0 ) );
        if ( xp >= 0 && xp < width[img] && yp >= 0 && yp < height[img] )
            image[img][ xp ][ yp ] = DARK_PIXEL;  /* 暗い画素 */
    }
}

double calc_distance( int x1, int y1, int x2, int y2 )
/* (x1,y1),(x2,y2) 間の距離を返す関数 */
{
	double dist;
	dist = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
	if (dist==0) 
	  return 1.0;
	else 
      return dist;
}

int random_int( int n )
/* 0 以上 n - 1 以下の整数の乱数を返す */
{
    return (int)( (double)rand( ) / ( (double)RAND_MAX + 1.0 ) * n );
}

void obtain_points_data(int img )
/* image1[y][x] 中の白画素のデータを points[ ][ ]に代入 */
{
    int x, y;   /* ループ変数 */

    num_pnts = 0;
    for ( y = 0; y < height[img]; y ++ ){
        for ( x = 0; x < width[img]; x ++ ){
            if ( image[img][x][y] == MAX_BRIGHTNESS ){
                if ( num_pnts < MAX_PNTS - 2 ){
                    num_pnts ++;
                    points[num_pnts - 1][0] = x;
                    points[num_pnts - 1][1] = y;
                }
            }
        }
    }
    printf("全部で %d 点のデータを取得しました．\n", num_pnts);
    if ( num_pnts < 5 ){
        printf("点列が少な過ぎます．画像を作り直して下さい．\n");
        exit( 1 );
    }
}

void shuffle_points_data( )
/* 取得した点データをランダムに入れ替える． */
{
    int seed;              /* 乱数のシード                 */
    int i, j, num1, num2;  /* 制御変数，入れ替える番号1，2 */
    int temp[2];           /* 入れ替えに用いる作業変数     */
    int max;    /* 入れ替え回数 */

    /* 乱数の初期化 */
    printf("乱数の系列を初期化します．\n");
    printf("乱数のシード（任意の正の整数）:");
    scanf("%d",&seed);
    if ( seed < 1 ) seed = 1;
    srand( seed );

    /* 点の総数の２倍の回数の入れ替え処理 */
    max = num_pnts * 2;
    for ( i = 0; i < max; i ++ ){
        num1 = random_int( num_pnts );
        do { 
            num2 = random_int( num_pnts );
        } while ( num2 == num1 );
        /* No.num1 と No.num2 の点を入れ替えます */
        for ( j = 0; j < 2; j ++ ){
            temp[j]         = points[num1][j];
            points[num1][j] = points[num2][j];
            points[num2][j] = temp[j];
        }
    }
}

void clustering_using_Kmeans(int src, int dst)
/* K-平均法によるクラスタリングを行う */
{
    int K_number;       /* K の値（生成するクラスタの総数） */
    int y, x, i, j;                 /* 制御変数など         */
    double min_distance, distance;  /* 距離に関する変数     */
    int min_num;        /* 点に対して最も近いクラスタの番号 */
    double sum_x, sum_y, sum_number;  /* クラスタの作業変数 */
    int init_cntr_clstr[MAX_PNTS][2]; /* クラスタの初期位置 */
    int radius_clstr[MAX_PNTS];   /* クラスタの意味上の半径 */
    int finish;                   /* 処理終了に関するフラグ */
    int counter;                  /* 処理回数               */

    printf("K-平均法によって点列をクラスタリングします．\n");
    printf("K（クラスタ総数）の値を入力して下さい．\n");
    do {
        printf("（ 1 以上 %d 以下 ）: ", num_pnts / 2);
        scanf("%d",&K_number);
    } while ( K_number < 1 || K_number > ( num_pnts / 2.0 ) );
    printf("データを分類してクラスタを形成し，結果を\n");
    printf("画像として作成します．\n");
    /* 番号はランダム順にされているので，No.0～K_number-1 */
    /* の点列を最初の K_number 個のクラスタ中心に設定する */
    num_clstr = K_number;
    for ( i = 0; i < num_clstr; i ++ )
        for ( j = 0; j < 2; j ++ )
            center_clstr[i][j] = points[i][j];
    finish = 0;    counter = 0;
    while ( ! finish ){
        counter ++;  
        printf("%d 回目のクラスタリング中です．\n", counter );
        /* クラスタの初期代表点への代入 */
        for ( i = 0; i < num_clstr; i ++ ){
            for ( j = 0; j < 2; j ++ )
                init_cntr_clstr[i][j] = center_clstr[i][j];
            radius_clstr[i] = 10; /* defalt 値 */
        }
        /* 各点の所属クラスタ番号の初期化 */
        for ( i = 0; i < num_pnts; i ++ )
            point_clstr[i] = - 1;
        /* 各点の所属クラスタを決定する */
        for ( i = 0; i < num_pnts; i ++ ){
            /* 各クラスタ代表点との距離を求める */
            min_distance = 10000.0;
            for ( j = 0; j < num_clstr; j ++ ){
                distance = calc_distance( points[i][0], points[i][1],
                           center_clstr[j][0], center_clstr[j][1] );
                if ( distance < min_distance){
                    min_distance = distance;
                    min_num = j;
                }
            }
            /* No.i の点をクラスタ No.min_num へ属させる */
            point_clstr[i] = min_num;
        }
        /* 各クラスタの代表点を修正する */
        for ( i = 0; i < num_clstr; i ++ ){
            sum_x = 0.0;    sum_y = 0.0;
            sum_number = 0.0;
            for ( j = 0; j < num_pnts; j ++ ){
                if ( point_clstr[j] == i ){
                    sum_number = sum_number + 1.0;
                    sum_x = sum_x + points[j][0];
                    sum_y = sum_y + points[j][1];
                }
            }
            center_clstr[i][0] = (int)( sum_x / sum_number );
            center_clstr[i][1] = (int)( sum_y / sum_number );
        }
        /* 各クラスタの意味上の半径を求める */
        for ( i = 0; i < num_clstr; i ++ ){
            for ( j = 0; j < num_pnts; j ++ ){
                if ( point_clstr[j] == i ){
                    distance = calc_distance( 
                        center_clstr[i][0], center_clstr[i][1],
                        points[j][0], points[j][1] );
                    if ( distance > radius_clstr[i] )
                        radius_clstr[i] = (int)distance;
                }
            }
        }
        /* クラスタ中心が移動したかどうかのチェック */
        finish = 1;
        for ( i = 0; i < num_clstr; i ++ ){
            distance = calc_distance( 
                init_cntr_clstr[i][0], init_cntr_clstr[i][1],
            center_clstr[i][0],    center_clstr[i][1] );
            if ( distance > 1.0 ) finish = 0;  /* 許容誤差 = 1.0 */
        }
        if ( finish == 1 ){  /* 処理終了 */ 
            /* image2[y][x] の初期化 */
            width[dst] = width[src];  height[dst] = height[src];
            for ( y = 0; y < height[dst]; y ++ )
                for ( x = 0; x < width[dst]; x ++ )
                    image[dst][x][y] = image[src][x][y];
            /* クラスタ中心と各点データまでの線分を描く   */
            for ( i = 0; i < num_pnts; i ++ ){
                draw_a_straight_line(dst, points[i][0], points[i][1],
                    center_clstr[ point_clstr[i] ][0],
                    center_clstr[ point_clstr[i] ][1] );
            }
            /* image2[y][x] にクラスタ中心と円を描く      */
            /* 円の半径＝そのクラスタに所属する最も遠い点 */
            /* までの距離(=radius_clstr[ ]) + 5           */
            for ( i = 0; i < num_clstr; i ++ ){
                printf("No.%d のクラスタの半径 = %d\n", 
                    i + 1, radius_clstr[i] );
                draw_a_circle(dst, center_clstr[i][0],
                    center_clstr[i][1], 5 );
                draw_a_circle(dst, center_clstr[i][0],
                    center_clstr[i][1], radius_clstr[i] + 5 );
            }
            /* image2[y][x] に元のデータ点を再描画する */
            for ( i = 0; i < num_pnts; i ++ )
                image[dst][ points[i][0] ][ points[i][1] ] =
                    MAX_BRIGHTNESS;
        }
    }
    printf("K-平均法によるクラスタリングが終了しました．\n");
}

int main( )
{
    load_image(1,"" ); /* 画像を読み込んで image1 へ */
    Hough_transform(1, 2); /* Hough 変換して結果を image2 へ */
    
    check_data(2);  /* データのチェック */
    inverse_Hough_transform(2, 1);  /* Hough逆変換して直線抽出 */

    obtain_points_data(1);        /* image1中の点をデータ化   */
    shuffle_points_data( );       /* データ順序のランダム化   */
    clustering_using_Kmeans(1, 3);   /* K-平均法でクラスタリング */
                                  /* してimage2へ描画する     */
    save_image(3,"" );           /* image2 --> ファイル出力  */
    return 0;
}
