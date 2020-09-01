#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
  long long int i, n=10000000000;
  double start_time, end_time;
  double x, pi;
  double sum = 0.0;
  double step = 1.0/(double) n;

//#pragma acc data copyin(step) copyout(sum,pi)
//  {
    start_time = omp_get_wtime();
//#pragma acc kernels loop private(i,x) reduction(+:sum)
#pragma omp parallel loop
      for (i = 0; i < n; i++) {
        x = (i+0.5)*step;
        sum +=  4.0/(1.0+x*x);
      }
    pi = step * sum;
    end_time = omp_get_wtime();
//  }
//pi = step * sum;
  printf("pi = %17.15f\n",pi);
  printf("time to compute = %g seconds\n", (double)(end_time - start_time));
  return 0;
}

/* 
Scalars are private by default so your "private" clause is unnecessary. Loop index variables like "i" are treated special so would be ignored when listed in a "private" clause.
Your code does have an error. The device "sum" is copied out of the data region after you use it on the host. Hence "pi" is using the host "sum", which is zero. Either update "sum" or move the computation of "pi" after your data region.
*/
