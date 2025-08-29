#include <stdio.h>
#define SIZE 5000

float a[SIZE][SIZE];
float b[SIZE][SIZE];
float c[SIZE][SIZE];
int main() {
int i, j, k;

    // Valor inicial da matriz
#pragma acc parallel loop collapse(2)
    for (i = 0; i < SIZE; ++i) {
      for (j = 0; j < SIZE; ++j) {
          a[i][j] = (float)i + j;
          b[i][j] = (float)i - j;
          c[i][j] = 0.0f;
      }
    } 

    // Calcula a soma da matriz
#pragma acc parallel loop collapse(2)
    for (i = 0; i < SIZE; ++i) {
       for (j = 0; j < SIZE; ++j) {
           c[i][j] = a[i][j] + b[i][j];
       }
    }

    return 0;
}
