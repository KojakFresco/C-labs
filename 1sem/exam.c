#include <stdio.h>
#include <math.h>

double curve_len(double (*f)(double), double a, double b, double h) {
    double x = a;
    double eps = 1e-15;
    double len = 0;
    while ((b - x) > eps) {
        double x1 = x;
        double x2 = fmin(x + h, b);
        double y1 = f(x1), y2 = f(x2);
        len += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
        x = x2;
    }
    return len;
}