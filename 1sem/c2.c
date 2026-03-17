#include <stdio.h>
#include <math.h>

double fun1(double x) {
    return exp(x) + log(x) - 10*x;
}

double fun1s(double x) {
    return (exp(x) + log(x))/10;
}

double fun21(double x) {
    return tan(x) - 1.0/3*pow(tan(x),3) + 1.0/5*pow(tan(x), 5) - 1.0/3;
}

double fun21s(double x) {
    return atan(1.0/3*pow(tan(x),3) - 1.0/5*pow(tan(x), 5) + 1.0/3);
}

double der(double (*fun)(double),  double x) {
    double dx = 10e-15;
    return (fun(x + dx) - fun(x)) / dx;
}

double sec_der(double (*fun)(double),  double x) {
    double dx = 10e-15;
    return (der(fun, x + dx) - der(fun, x)) / dx;
}
double newton(double (*fun)(double), double a, double b) {
    double eps = 10e-15;
    double x = (a + b) / 2;

    if (fabs(fun(x) * sec_der(fun, x)) >= pow(der(fun, x), 2)) {
        return -1.0/0;
    }

    double x1 = x - fun(x)/der(fun, x);
    while (!(fabs(x - x1) < eps)) {
        x = x1;
        x1 = x - fun(x)/der(fun, x);
    }
    return x1;
}

double iters(double (*fun)(double), double a, double b) {
    double eps = 10e-15;
    double x = (a + b) / 2;

    if (fabs(der(fun, x)) >= 1) {
        return -1.0/0;
    }

    double x1 = fun(x);
    while (!(fabs(x - x1) < eps)) {
        x = x1;
        x1 = fun(x);
    }
    return x1;
}

double dichotomy(double (*fun)(double), double a, double b) {
    double eps = 10e-15;
    while (!(fabs(a - b) < eps)) {
        double m = (a + b) / 2;
        if (fun(a)*fun(m) > 0) {
            a = m;
        } else {
            b = m;
        }
    }
    return (a + b) / 2;
}
int main(void) {
    printf("  dichotomy  iterations  newton\n");
    printf("1 %9.4lf  %10.4lf  %6.4lf\n", dichotomy(fun1, 3, 4), iters(fun1s, 3, 4), newton(fun1, 3, 4));
    printf("2 %9.4lf  %10.4lf  %6.4lf\n", dichotomy(fun21, 0, 0.8), iters(fun21s, 0, 0.8), newton(fun21, 0, 0.8));
    return 0;
}