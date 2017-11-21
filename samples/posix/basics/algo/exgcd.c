#include <stdio.h>

int exgcd(int m,int n,int *s,int *t)
{
    int x0 = 1;
    int y0 = 0;
    int x1 = 0; 
    int y1 = 1;
    int r = m % n;
    int q = (m - r) / n;
    *s = 0; 
    *t = 1;

    while(r)
    {
        *s = x0 - (q * x1); 
        *t = y0 - (q * y1);
        x0 = x1; 
        y0 = y1;
        x1 = *s; 
        y1 = *t;
        m = n;
        n = r;
        r = m % n;
        q = (m - r) / n;
    }
    return n;
}

/** 
*   示例来自《深入浅出密码学 p154 示例 6.6》 
*   tools: http://people.math.sc.edu/sumner/numbertheory/euclidean/euclidean.html
*/
int main(int argc, char const *argv[])
{
    int s, t;
    int m = 67;
    int n = 12;

    int g = exgcd(m, n, &s, &t);
    printf("  exgcd(%d, %d) = %d  ==>  [s = %d, t = %d]\n", m, n, g, s, t);

    return 0;
}