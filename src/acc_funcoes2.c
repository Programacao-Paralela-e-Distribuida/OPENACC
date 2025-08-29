#include<stdio.h>
#ifdef _OPENACC
#include <openacc.h>              // biblioteca OpenACC 
#endif
#include <omp.h>
int main(int argc, char *argv[]) { /* acc_funcoes2.c   */

#pragma omp parallel num_threads(2)
{
    int tid = omp_get_thread_num();
#ifdef _OPENACC
    acc_init(acc_device_nvidia); // chamada OpenACC
    const int num_dev = acc_get_num_devices(acc_device_nvidia); //#GPUs
    const int dev_id = tid%num_dev;
    // atribui uma GPU para cada thread OpenMP
    acc_set_device_num(dev_id, acc_device_nvidia);   
    printf ("A thread %d é atribuída à GPU %d \n", tid, dev_id);
#endif
}
}
