#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define SET_G_ENABLE 1
#define XTIME(x) ( ( x << 1 ) ^ ( ( x & 0x80 ) ? 0x1B : 0x00 ) )

#define TABLES_HEARD    \
"   | 0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f \n\
---|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|"

void dump_tables(char *info, int *tables, int len)
{
    printf("%s\n%s\n", info, TABLES_HEARD);
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

uint8_t gmul(uint8_t a, uint8_t b) 
{
    uint8_t p = 0;
    uint8_t hbit;

    for(uint8_t i = 0; i < 8; i++) 
    {
        if((b & 1) == 1)
            p ^= a;

        hbit = (a & 0x80);
        a <<= 1;

        if(hbit == 0x80)
            a ^= 0x1b;

        b >>= 1;
    }

    return p;
}

#if SET_G_ENABLE
/*
 * compute pow, log, inv tables over GF(2^8)
 */
void gen_tables(int g, int *pow, int *log, int *inv, int len)
{
    int i, x = g;
    pow[0] = 1; pow[1] = x;
    log[0] = 0; log[1] = 0xff;

    for(i = 2; i < len; i++)
    {
        x = gmul(x, g);
        pow[i] = x;
        log[x] = i;
    }

    for(i = 1; i < len; i++)
    {
        inv[i] = pow[(255 - log[i])];
    }
}
#else
/*
 * compute pow, log, inv tables over GF(2^8)
 */
void gen_tables(int *pow, int *log, int *inv, int len)
{
    int i, x;
    for( i = 0, x = 1; i < len; i++ )
    {
        *(pow + i) = x;
        *(log + x) = i;
        x = ( x ^ XTIME( x ) ) & 0xFF;
    }  

    for(i = 1; i < len; i++)
    {
        inv[i] = pow[(255 - log[i])];
    }  
}
#endif

/** 
*   http://www.samiam.org/logtables.txt  Generator: 0x03
*   http://www.samiam.org/galois.html  Exponents and logarithms
*/
int main(int argc, char const *argv[])
{
    uint8_t a = 0x03;
    uint8_t b = 0x03;
    printf("gmul(%02x, %02x) = %02x\n\n", a, b, gmul(a, b));

    int pow[256];
    int log[256];
    int inv[256];

    memset(pow, 0, sizeof(pow));
    memset(log, 0, sizeof(log));
    memset(inv, 0, sizeof(inv));

#if SET_G_ENABLE
    gen_tables(0x05, pow, log, inv, 256);
#else
    gen_tables(pow, log, inv, 256);
#endif

    dump_tables("Log table:", log, 256);
    dump_tables("Pow table:", pow, 256);
    dump_tables("Inv table:", inv, 256);

    return 0;
}