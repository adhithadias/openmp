#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/times.h>
#include <stdlib.h>

#define N   1000000
#define NUM_THREADS 20
#define MUL_THREAD  8

void initialze_array(const int n, float *a, const float value);
void print_array(const int n, const float *a);

void initialze_array(const int n, float *a, const float value) {
    for (int i = 0; i < n; i++ ) {
      a[ i ] = value; 
   }
}

void print_array(const int n, const float *a) {
    printf("[");
    for (int i=0; i<10; i++){
        printf("%f ", a[i]);
    }
    printf("]\n");
}

void print_thread_info(int i) {
    int curr_thread, nthread;
    int len = 20;
    char name[20];
    curr_thread = omp_get_thread_num();
    nthread = omp_get_num_threads();
    gethostname(name, len);
    printf("name: %s, thread_count: %d, current_thread: %d, i: %d\n", name, nthread, curr_thread, i);
    return;
}

/*  Example to check sequential execution and parallel execution times of 
*	OpenMP. The method print out 3 execution times.
*/
void reduction() {

    printf("Starting the reduction script\n");
    clock_t start, end;		// for keeping track of the cpu time/cpu clock cycles
    double start_t, end_t;	// to calculate actual execution time

    printf("\n\n==============INITIALITING THE ARRAY=================\n");
    float *x = (float *) malloc(N*sizeof(float));
    initialze_array(N, x, 1.0);
    float y = 0.0;

    printf("\n============PART 1: SEQUENTIAL EXECUTION===============\n");
    start = clock();
    start_t = omp_get_wtime(); 
    for(int i=0; i<N; i++) {
        y += x[i];
    }
    end_t = omp_get_wtime(); 
    end = clock();
    printf("SERIAL REDUCTION | answer: %f, cpu_time: %f, execution_time: %f\n", y, (end-start)/(double)CLOCKS_PER_SEC, end_t-start_t);

    printf("\n\n==============PART 2: PARALLEL EXECUTION===============\n");

    omp_set_num_threads(NUM_THREADS);

    y = 0.0;
    float res[NUM_THREADS*MUL_THREAD];
    initialze_array(NUM_THREADS*MUL_THREAD, res, 0.0);
    int i=0;
    int count = 0;
    
    start = clock();
    start_t = omp_get_wtime(); 
    #pragma omp parallel for shared(res)
    for (i=0; i < N; i++) {
        res[omp_get_thread_num()*MUL_THREAD] += x[i];
    }
    end_t = omp_get_wtime(); 
    end = clock();
    
    clock_t inter = end-start;
    double inter_t = end_t-start_t;
    
    start = clock();
    start_t = omp_get_wtime(); 
    for (i=0; i<MUL_THREAD*NUM_THREADS; i+=MUL_THREAD) {
        y += res[i];
    }
    end_t = omp_get_wtime(); 
    end = clock();

    printf("PARTIAL PARALLEL REDUCTION | answer: %f, cpu_time: %f, execution_time: %f\n", y, inter/(double)CLOCKS_PER_SEC, inter_t);
    printf("PARTIAL SERIAL REDUCTION | answer: %f, cpu_time: %f, execution_time: %f\n", y, (end-start)/(double)CLOCKS_PER_SEC, end_t-start_t);
    printf("TOTAL TIME | answer: %f, cpu_time: %f, execution_time: %f\n", y, (inter+end-start)/(double)CLOCKS_PER_SEC, inter_t+end_t-start_t);


    printf("\n\n==============PART 3: OpenMP REDUCTION=================\n");
    y = 0.0;

    start = clock();
    start_t = omp_get_wtime(); 
    #pragma omp parallel for shared(x) reduction(+:y)
    for (int i=0; i<N; i++) {
        y += x[i];
    }
    end_t = omp_get_wtime(); 
    end = clock();
    
    printf("COMPLETE PARALLEL REDUCTION | answer: %f, cpu_time: %f, execution_time: %f\n", y, (end-start)/(double)CLOCKS_PER_SEC, end_t-start_t);

    free(x);
    return;

}

int main (int argc, char *argv []) {

    reduction();
    return 0;
}
