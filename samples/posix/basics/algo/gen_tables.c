#include <stdio.h>
#include <string.h>

#define XTIME(x) ( ( x << 1 ) ^ ( ( x & 0x80 ) ? 0x1B : 0x00 ) )

/*
 * compute pow and log tables over GF(2^8)
 */
void gen_low_pow_tables(int *pow, int *log, int len)
{
    int i, x;
    for( i = 0, x = 1; i < len; i++ )
    {
        *(pow + i) = x;
        *(log + x) = i;
        x = ( x ^ XTIME( x ) ) & 0xFF;
    }    
}

#define TABLES_HEARD    \
"   | 0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f \n\
---|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|"

void dump_tables(char *info, int *tables, int len)
{
    printf("%s\n", info);
    printf("%s\n", TABLES_HEARD);

    for(int i = 0; i < len/16; i++)
    {
        printf("%02x |", i*16);
        for(int j = 0; j < 16; j++)
        {
            printf("%02x ", *(tables + j + (i*16)));
        }
        printf("\n");
    }
    printf("\n");
}

/** 
*   http://www.samiam.org/logtables.txt  Generator: 0x03
*/
int main(int argc, char const *argv[])
{
    int pow[256];
    int log[256];
    memset(pow, 0, sizeof(pow));
    memset(log, 0, sizeof(log));

    gen_low_pow_tables(pow, log, 256);

    dump_tables("Log table:", log, 256);
    dump_tables("Pow table:", pow, 256);

    return 0;
}