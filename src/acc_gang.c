#include <stdlib.h>
void saxpy(int n, float a, float * restrict x, float * restrict y) {
    /* Divide as iterações entre as gangues */
    #pragma acc parallel num_gangs(10)
    #pragma acc loop gang (static:10000)
    for (int i = 0; i < n; ++i)
        y[i] = a*x[i] + y[i];
}
int main(int argc, char *argv[]) { /* acc_gang.c  */
int n = 1<<24;                     /* 16 milhões de floats */
float *x = (float*)malloc(n*sizeof(float));
float *y = (float*)malloc(n*sizeof(float));
    /* Define o número de trabalhadores e o tamanho do vetor */
    #pragma acc parallel num_workers(8) vector_length(128)
    /* Divide as iterações entre as gangues, workers e vetores */
    #pragma acc loop gang worker vector
    for (int i = 0; i < n; ++i) {
        x[i] = 2.0f;
        y[i] = 1.0f;
    }
    saxpy(n, 3.0f, x, y);
    free(x);
    free(y);
    return(0);
}
