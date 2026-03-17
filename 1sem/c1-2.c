#include <stdio.h>

int sum(int a, int b) {
    int ans = 0;
    int k;
    int k_bit = 0;
    int ost = 0;
    for (k = 0; k <= 23; k++) {
        k_bit = ((a >> k) & 1) + ((b >> k) & 1) + ost;
        ans |= (k_bit & 1) << k;
        ost = (k_bit >> 1) & 1;
    }
    ans |= ost << k;
    return ans;
}
int main() {
    float a;
    scanf("%f", &a);
    int b = *(int*)&a;
    int s = b >> 31;
    int e = b >> 23 & 0xff;
    int m = b & (1 << 23) - 1;

    int m01 = m >> 1;
    m = sum(1 << 22, sum(m, m01));
    e += 1 + (m >> 23);
    m = (m &(1 << 23) - 1) >> (m >> 23);

    int ans = s << 31 | e << 23 | m;
    float f_ans = *(float*)&ans;
    printf("%f", f_ans);
    return 0;
}