#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
int main( int argc, char* argv[] ) /* acc_somavetor.c  */
{
int n = 10000;                     /* tamanho dos vetores  */
double *restrict a;                /* vetores de entrada */
double *restrict b;
double *restrict c;               /* vetor de saída */
size_t bytes = n*sizeof(double);  /* tamanho em bytes de cada vetor */

    /*  Alocação de memória para cada vetor */
    a = (double*)malloc(bytes);
    b = (double*)malloc(bytes);
    c = (double*)malloc(bytes);
 
    /* Atribui os valores iniciais para os vetores de entrada */ 
    int i;
    for (i = 0; i < n; i++) {
         a[i] = sin(i)*sin(i);
         b[i] = cos(i)*cos(i);
    }
 
    /*  Soma os elementos de cada vetor e salva o resultado no vetor C e realiza a redução */
    float sum = 0.0;
    #pragma acc kernels copyin(a[0:n],b[0:n]), create(c[0:n]) 
    {
        for(i = 0; i < n; i++) {
            c[i] = a[i] + b[i];
            sum += c[i];
        }
    }
 
    sum = sum/(double)n;
    printf("resultado final: %f\n", sum);
 
    /* Libera a memória alocada */
    free(a);
    free(b);
    free(c);
 
    return 0;
}
