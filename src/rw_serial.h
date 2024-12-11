/* rw_serial.h  このプログラムの名前 */
/* 中で文字列操作関数を用いているので string.h が必要 */
/* 中で itoa を用いているので stdlib.h が必要 */

/* 連続ファイル名のパラメータを指定する関数 */
void set_serial_parameters( int *st, int *end, int *keta, 
    char inh[], char outh[], int *w, int *h );
/* 具体的なファイル名を返す関数 */
void make_file_name( char hdr[], int num, int keta, char str[] );


void itoa(int value, char *buf);
char *strrev(char *s);

void set_serial_parameters( int *st, int *end, int *keta, 
    char inh[], char outh[], int *w, int *h )
/* st:開始フレーム，end:最終フレーム，keta:桁数（例:0001→keta=4) */
/* inh[]:入力画像のヘッダ部分，outh[]:出力画像のヘッダ部分 */
/* w:画像の横幅，h:画像の縦幅 */
/* file0001<==>file0010 >>> out0001<==>out0010 の場合： */
/* st=1, end=10, keta=4, inh[]="file", out[]="out" */
{
    int ys;
    char fn1[256],fn2[256];

    printf("<< 連続画像のパラメータを指定します >>\n");
    do{
        printf("●使用する画像を指定します\n");
        printf("　開始番号：");  scanf("%d",st);
        printf("　終了番号：");  scanf("%d",end);
        printf("　桁数(ex. 00000 のとき 5)：");  scanf("%d",keta);
        printf("　入力：数字の前に文字列を入れる(1)/入れない(0)：");
        scanf("%d",&ys);
        if (ys==1){
            printf("　入力画像のヘッダー：");  scanf("%s",inh);
        } else inh[0]='\0';
        printf("　出力：数字の前に文字列を入れる(1)/入れない(0)：");
        scanf("%d",&ys);
        if (ys==1){
            printf("　出力画像のヘッダー：");  scanf("%s",outh);
        } else outh[0]='\0';
        make_file_name( inh, *st, *keta, fn1 );
        make_file_name( inh, *end, *keta, fn2 );
        printf("　入力画像：%s ===> %s\n", fn1, fn2);
        make_file_name( outh, *st, *keta, fn1 );
        make_file_name( outh, *end, *keta, fn2 );
        printf("　出力画像：%s ===> %s\n", fn1, fn2);
        printf("よろしいですか(1:ok, 0:not ok):");
        scanf("%d",&ys);
    } while( ys != 1 );
    /* 以下では試しに1枚読み込んで画像サイズを調べている */
    make_file_name( inh, *st, *keta, fn1 );  load_image( 0, fn1 );
    *w = width[0];  *h = height[0];
}

void make_file_name( char hdr[], int num, int keta, char str[] )
/* ファイル名：hdr[]000****.pgm を作り，str[] に代入 */
{
    char buffer[20];
    int length,i;

    itoa( num, buffer );  /* num を10進数にして buffer[] へ */
    length = strlen(buffer);   /* 何文字かを調べる */
    if (hdr != "") strcpy( str, hdr );
    for(i=1;i<=keta-length;i++)  strcat( str, "0" );
    strcat( str, buffer );
    strcat( str, ".pgm" );
}

//
//  関数の追加　itoa
//
void itoa(int value, char *buff)
{
	char *p;
	int mod;

	// 入力した値の正負を文字にする。
	if( 0 > value ) {
		*buff++ = '-';
		value = -value;
	}
	p = buff;

// 入力した値を下位の桁から順に文字にする。
	do{
		mod = value % 10 + 48;
		*p++ = (char)mod;
		value /= 10;
	}while(value);

	*p = '\0';// 文字列の最後に '\0' を追加して締める。

	strrev(buff);// 文字列の上位の桁から始まるように並べ替える。

}

char *strrev(char *s)
{
	char *ret = s;
	char *t = s;
	char c;

	while( *t != '\0' )
		t++;
		t--;

		while(t > s) {
			c = *s;
			*s = *t;
			*t = c;
			s++;
			t--;
		}

		return ret;
}
	
