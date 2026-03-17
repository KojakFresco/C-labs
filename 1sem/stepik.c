#include <stdio.h>
#include <math.h>
int main(void)
{
    double e = 1, exp; int i = 2, f = 1;
    scanf("%lf", &exp);
    while (f <= 1/exp) {
        e += 1.0/f;
        f *= i;
        i++;
    }
    printf("%lf", e);
    return 0;
}