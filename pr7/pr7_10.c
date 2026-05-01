#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    double n;
    printf("Enter n: ");
    scanf("%lf", &n);
    srand(time(NULL));
    double r1 = (double)rand() / RAND_MAX;
    double r2 = r1 * n;
    printf("Range 0.0 - 1.0: %f\n", r1);
    printf("Range 0.0 - %f: %f\n", n, r2);
    return 0;
}