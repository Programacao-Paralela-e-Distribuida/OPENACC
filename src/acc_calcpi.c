#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) { /* acc_calcpi.c  */
double pi = 0.0f; 
long N=1000000000;

    double inicio = omp_get_wtime();
    #pragma acc parallel loop reduction(+: pi)
    for (long i = 0; i < N; i++) {
        double t=(double) ((i+0.5)/N);
        pi += 4.0/(1.0+t*t);
   	}
    double fim = omp_get_wtime();
    printf("O valor de pi é: %f\n",pi/N);
    printf("O tempo de execução é %f segundos.\n",fim-inicio);
    return(0);
}
