#include <stdio.h>
#include <math.h>
#include <omp.h>
double f(double x) {
double valor;
    valor = exp(x);
    return(valor);
}
int main(int argc, char *argv[]) { /* acc_trapezio2.c  */
double integral;         /* armazena resultado em integral */
double a, b;             /* limite esquerdo e direito */
long i,n;                /* número de trapezóides */
double h;                /* largura da base do trapezóide */

    a = 0.0;
    b = 1.0;
    n = 800000000;
    h = (b-a)/n;
#pragma acc data copy(integral)
    integral = (f(a) + f(b))/2.0;
    double inicio = omp_get_wtime(); /* vamos usar a função do OpenMP */
#pragma acc parallel loop reduction(+:integral)
    for (i = 1; i < n-1; i++) {
        integral += f(a + i*h);
    }
    integral *= h ;
    printf("Com n = %ld trapezoides, a estimativa \n", n);
    printf("da integral de %f ate %f = %lf \n", a, b, integral);
    printf("Tempo: \t %f \n", omp_get_wtime()-inicio);
    return(0);
}
