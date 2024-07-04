/* ランレングス符号化を行うプログラム rlcode.c */
#include<stdio.h>
#include<stdlib.h>
#include"pgmlib.h"

unsigned char line_buffer[5000]; /* 1ライン分のデータ代入用 */
int index1;          /* line_buffer[ ] の現在の要素番号を表す */
FILE *fp;          /* データを出力する際のファイルポインタ  */

void Wyle_coding( int n )
/* 長さ n のランレングス符号を求めて line_buffer[ ] に代入 */
{
    int n_length;     /* n - 1 の２進桁数   */
    int i, n_1, work; /* 制御・作業変数     */

    /* ランに対する符号長 n_length を求める */
    n_1 = n - 1;
    if ( n_1 <= 3 ) n_length = 2;
    else {
        work = n_1;  n_length = 1;
        do { 
            n_length ++;    work = work / 2;
        } while ( work > 1 );
    }
    /* Wyle 符号を代入する */
    if ( n_length > 2 ){
        for ( i = 0; i < n_length - 2; i ++ ){
            line_buffer[index1] = 1;    index1 ++;
        }
    }
    line_buffer[index1] = 0;
    /* n - 1 のバイナリ符号を代入する */
    index1 = index1 + n_length;
    do {
        line_buffer[index1] = (unsigned char)(n_1 % 2);
                                   /* 2で割ったときの余り */
        n_1 = n_1 / 2;    index1 --;
    } while ( n_1 > 1 );
    if ( n < 3 ) line_buffer[index1] = 0;
    else line_buffer[index1] = 1;
    index1 = index1 + n_length;
}

void save_line_buffer( )
/* line_buffer[ ] の内容をファイルに出力する */
{
    unsigned char byte_data; /* 1バイト分のデータ用 */
    int byte_index1;     /* line_buffer[ ] 内要素番号 */
    int byte_size,remain_bit_size;  /* 符号の長さ用 */
    int i, j;  /* 制御変数 */

    /* 符号長は全部で index1 ビット */
    byte_size = index1 / 8;        
    remain_bit_size = index1 % 8;  /* 端数のビット数 */
    byte_index1 = -1;
    for ( i = 0; i < byte_size; i ++ ){
        byte_data = 0;
        for ( j = 1; j <= 8; j ++ ){
            byte_index1 ++;
            byte_data = (unsigned char)(byte_data * 2
                + line_buffer[byte_index1]);
        }
        fputc( byte_data, fp );  /* ファイル出力 */
    }
    /* 以下は端数のビットがあるときの処理 */
    if ( remain_bit_size != 0 ){
        byte_data = 0;
        for ( j = 1; j <= remain_bit_size; j ++ ){
            byte_index1 ++;
            byte_data = (unsigned char)(byte_data * 2
                + line_buffer[byte_index1]);
        }
        byte_data = (unsigned char)( byte_data <<
            ( 8 - remain_bit_size ) );
        fputc( byte_data, fp );  /* ファイル出力 */
    }
}

void code_and_save_runlength_data(int n )
/* 画像のランレングス符号化を行ってデータを保存する */
{
    int y, x; /* 制御変数 */
    long int coded_data_size;     /* 圧縮データ量   */
    char file_name[MAX_FILENAME]; /* 出力ファイル名 */
    int run_length;               /* ランレングス   */

    printf("原画像をランレングス符号化してデータを保存します．\n");
    printf("出力ファイル名 (*.wyl) : ");
    scanf("%s",file_name);    fp = fopen( file_name, "wb");
    /* 画像の横幅，縦幅の出力 */
    fprintf( fp, "%d %d\n", width[n], height[n] );
    /* ランレングス符号化 */
    coded_data_size = 0;
    for ( y = 0; y < height[n]; y ++ ){
        /* 最左の画素の階調値を１ビットで表現する */
        if ( image[n][0][y] == 0 ) line_buffer[0] = 0;
        else line_buffer[0] = 1;
        index1 = 1; /* 次に代入すべき line_buffer[ ] の要素番号 */
                  /* であり，関数 Wyle_coding 内で更新される  */
        /* 横１ライン分の符号化 */
        x = 0;  run_length = 1;  /* 変数の初期化 */
        while ( x < width[n] ){
                /* 最右画素でない画素に対する処理 */
            if ( x != width[n] - 1 ){
                if ( image[n][x][y] == image[n][x+1][y] ){
                    run_length ++;
                } else {
                    Wyle_coding( run_length );
                    run_length = 1;
                }
            } else {
                /* 最右画素のときの処理 */
                if ( image[n][x][y] == image[n][x-1][y] ){
                    Wyle_coding( run_length );
                } else {
                    Wyle_coding( 1 ); 
                }
            }
            x ++;
        } /* 最右に来たら終了 */
        /* line_buffer[ ] のデータを出力する */
        save_line_buffer( );
        /* 圧縮データ量の更新 */
        coded_data_size = coded_data_size + index1;
    }
    fclose( fp );
    /* データ量・圧縮率関係の計算と表示 */
    printf("元の２値画像のデータ量 = %d x %d = %d bits\n",
        width[n],height[n],width[n] * height[n]);
    printf("圧縮データ（画像部分）のデータ量 = %d bits\n",
        coded_data_size);
    printf("圧縮率：%f[％]\n", 100.0 * coded_data_size / 
        (width[n] * height[n]) );
}

main( )
{
    load_image( 0,"");              /* 原画像の読み込み */
    code_and_save_runlength_data(0); /* コード化と保存   */
    return 0;
}
