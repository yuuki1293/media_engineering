/* labeling.h  このプログラムの名前 */
int label[MAX_IMAGESIZE][MAX_IMAGESIZE]; /* ラベル用 */

int labeling( int n );
int _search_4neighbors( int x, int y, int n );
void _modify_label( int num1, int num2, int n );

void _modify_label( int num1, int num2, int n )
/* label[][]中の全ての num1 を num2 に変更 */
{
    int x,y;

    for(y=0;y<height[n];y++)
        for(x=0;x<width[n];x++)
            if ( label[x][y]==num1) label[x][y]=num2;
}

int _search_4neighbors( int x, int y, int n )
/* label[x][y]の４近傍のラベルの最大値を返す関数 */
/* n は対象画像の番号（内外部判定のために使う）  */
{
    int max=0;
    
    if (y-1>=0 && label[x][y-1]>max )        max=label[x][y-1]; /* 上 */
    if (x-1>=0 && label[x-1][y]>max )        max=label[x-1][y]; /* 左 */
    if (y+1<height[n] && label[x][y+1]>max ) max=label[x][y+1]; /* 下 */
    if (x+1<width[n]  && label[x+1][y]>max ) max=label[x+1][y]; /* 右 */
    return max;
}

int labeling( int n )
/* 画像No.n 中の黒い図形にラベリングを施して，結果を大域変数 */
/* label[][]に代入する．戻り値は最終的なラベルの最大値．     */
{
    int x,y,num;
    int count=0; /* ラベル最大値 */
    int new_count;

    /* ラベルを初期化 */
    for(y=0;y<height[n];y++)
        for(x=0;x<width[n];x++)
            label[x][y]=0; /* 0:未処理を表す */
    /* 画像No.nをスキャン */
    for(y=0;y<height[n];y++)
        for(x=0;x<width[n];x++){
            if ( image[n][x][y] == 0 && label[x][y] == 0 ){
                num = _search_4neighbors(x,y,n);
                if (num==0)  /* 新しい孤立領域 */
                    label[x][y]=++count; /* countに+1してから代入 */
                else label[x][y]=num;
            }
        }
    /* label[][]を整形する */
    if ( count > 0 ){
        /* 重複の削除 */
        for(y=0;y<height[n];y++)
            for(x=0;x<width[n];x++)
                if (label[x][y]!=0){
                    num = _search_4neighbors(x,y,n);
                    if ( num > label[x][y]){
                        /* ラベルnum を全て変更 */
                        _modify_label(num,label[x][y],n);
                    }
                }
       /* 空いた番号を詰める */
        new_count=0;
        for(y=0;y<height[n];y++)
            for(x=0;x<width[n];x++)
                if ( label[x][y] > new_count ){
                    new_count++;
                    _modify_label(label[x][y],new_count,n);
                }
        return new_count;  /* ＝最終的なラベルの最大値 */
    } else return 0;  /* ＝孤立領域なし */
}
