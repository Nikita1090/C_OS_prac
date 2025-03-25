#include <stdio.h>
#include <limits.h>

int main(){
    double eps;
    scanf("%lf", &eps);
    double x;
    while (scanf("%lf", &x) != EOF){
        printf("%g", x);
        double x_p = 1;
        double x_n = (x_p + x/x_p) / 2;
        while  ((x_p - x_n) * (x_p - x_n) >= eps*eps){
            x_p = x_n;
            x_n = (x_p + x/x_p) / 2;
        }
        printf("\n%f", x_n);
        printf("\n%g", x_n);
        printf("\n%e", x_n);
        printf("\n%.10g\n", x_n);
    }
}