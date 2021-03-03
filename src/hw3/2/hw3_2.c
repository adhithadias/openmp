#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define N 100000

void add_array(int *x, int size);

int main(int argc, int *argv[]) {

    int *x = (void *) malloc(N*sizeof(int));
    for (int i=0; i<N; i++) { x[i] = 1; }

    printf("\n=========== MASTER CONSTRUCT ============\n");
    #pragma omp parallel
    {
        #pragma omp master
        add_array(x, N);
    }

    printf("\n=========== CRITICAL CONSTRUCT ============\n");
    #pragma omp parallel
    {
        #pragma omp critical
        add_array(x, N);
    }

    printf("\n=========== SINGLE CONSTRUCT ============\n");
    #pragma omp parallel
    {
        #pragma omp single
        add_array(x, N);
    }



}

void add_array(int *x, int size) {
    int sum = 0;
    for (int i = 0; i<size; i++) {
        sum += x[i];
    }
    printf("Thread num: %d, array_address: %p, sum: %d\n", omp_get_thread_num(), x, sum);
    return;
}
