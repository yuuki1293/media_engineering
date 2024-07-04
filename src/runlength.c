/* runlength.c  このプログラムの名前 */
#include<stdio.h>
#define N 16
int bitmask[N];
void make_bitmask(int n);
int make_runlength_code( int num );  

int main(void)
{
    int i;
    
    make_bitmask(N);
    for(i=1;i<=33;i++){
        printf("%2d: ",i);
        printf(" ==> %dビット\n",make_runlength_code( i ));
    }
    return 0;
}

void  make_bitmask(int n)
/* 1～nビットまでのビットマスクを求める */
{
    int i;
    
    bitmask[0]=1;
    for(i=1;i<n;i++) bitmask[i] = bitmask[i-1] * 2;
}

int make_runlength_code( int num )
/* 整数numをランレングス符号化する関数 */
{
    int n1=N,n2,i;

    num = num - 1;  /* まず1を引く */
    /* ランレングス部分のビット数 = n1 */
    if (num < 2) n1=2;
    else{
        do{
            n1--;
        }while( bitmask[n1]>num );
        n1++;
    }
    /* Wyle符号の割り当て */
    if (n1 == 2) n2=1; else n2 = n1 - 1;
    /* Wyle符号の表示 */
    if (n2==1) printf("0");
    else {
        for(i=1;i<n2;i++) printf("1");  printf("0");
    }
    printf("+");
    /* ランレングス部分の表示 */
    if (num == 0) printf("00"); else
    if (num == 1) printf("01"); else
    for(i=n1-1;i>=0;i--)
        printf("%d", (num & bitmask[i])/bitmask[i]);
    /* 合計ビット数のリターン */
    return n1+n2;
}
