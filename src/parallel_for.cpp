#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <algorithm>
#include "util.h"

#define N 10

int len = 30;
char name[30];

int nthread;
int curr_thread;

void print_thread_info(void);
void print_thread_info(int i);


/*
*   i is private by default
*   threads share the loop 
*   same thread can run multiple iterations and there can be threads that are idle
*   run the script to understand the behaviour
*/
void patallel_exec(int n, float *a, float *b) {
    int i;

#pragma omp parallel for
    for(i=1; i<n; i++) {
        b[i] = (a[i] + a[i-1]) / 2.0;
        print_thread_info(i);
    }

    return;
}

int main (int argc, char *argv []) {

    print_thread_info();

    #pragma omp parallel private(name, nthread, curr_thread)
    {
        print_thread_info();
    }

    printf("\n=============== STARTING PARALLEL FOR SECTION ====================\n");

    float *a = (float *) malloc(N*sizeof(float));
    float *b = (float *) malloc(N*sizeof(float));

    initialze_array(N, a, 5);
    initialze_array(N, b, 10);

    patallel_exec(N, a, b);


    print_array(N, a);
    print_array(N, b);

    free(a);
    free(b);

    return 0;
}



void print_thread_info(void) {
    curr_thread = omp_get_thread_num();
    nthread = omp_get_num_threads();
    gethostname(name, len);
    printf("name: %s, thread_count: %d, current_thread: %d\n", name, nthread, curr_thread);
    return;
}

void print_thread_info(int i) {
    curr_thread = omp_get_thread_num();
    nthread = omp_get_num_threads();
    gethostname(name, len);
    printf("name: %s, thread_count: %d, current_thread: %d, i: %d\n", name, nthread, curr_thread, i);
    return;
}
