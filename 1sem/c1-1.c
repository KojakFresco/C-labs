#include <stdio.h>
#include <math.h>

int main() {
    unsigned int a, b;
    scanf("%u %u", &a, &b);
    unsigned int k = 0;
    unsigned int ans = 0;
    unsigned int k_bit = 0;
    while (a > 0 || b > 0) {
        k_bit += a % 10 + b % 10;
        ans += (k_bit % 3) * (int)pow(10, k);
        k_bit /= 3;
        a /= 10;
        b /= 10;
        k++;
    }
    ans += k_bit * (int)pow(10, k);
    printf("%d", ans);
    return 0;
}