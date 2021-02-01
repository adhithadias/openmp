#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/times.h>
#include <stdlib.h>

#define N 10000000
#define NUM_THREADS 8

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

/*  Example to check master construct and single construct directives
*   single and master constructs when specified, the statement is executed by only one thread
*   single construct, unless otherwise specified with nowait clause, wait at an implicit barrier
*   at the end of the single construct
*/
void reduction() {

    clock_t start, end;
    double start_t, end_t;
    

    printf("\n==============PART 1: SEQUENTIAL EXECUTION=================\n");
    double summation = 0.0;

    start = clock();
    start_t = omp_get_wtime(); 
    for(int i=1; i<N; i++) {
        summation += 1.0/i;
    }
    end_t = omp_get_wtime(); 
    end = clock();
    printf("Answer: %f, cpu time: %f, calculation time: %f\n", summation, (end-start)/(double)CLOCKS_PER_SEC, end_t-start_t);

    printf("\n==============PART 2: SEQUENTIAL  EXECUTION=================\n");

    summation = 0.0;
    
    start = clock();
    start_t = omp_get_wtime(); 
    for (int i=1; i < N; i++) {
        summation += 1.0/i;
    }
    end_t = omp_get_wtime(); 
    end = clock();

    printf("Answer: %f, cpu time: %f, calculation time: %f\n", summation, (end-start)/(double)CLOCKS_PER_SEC, end_t-start_t);


    printf("\n==============PART 3: OpenMP REDUCTION=================\n");

    omp_set_num_threads(NUM_THREADS);
    summation = 0.0;
    start = clock();
    start_t = omp_get_wtime(); 
    #pragma omp parallel for reduction(+:summation)
    for (int i=1; i<N; i++) {
        summation += 1.0/i;
    }
    end_t = omp_get_wtime(); 
    end = clock();
    printf("Answer: %f, cpu time: %f, calculation time: %f\n", summation, (end-start)/(double)CLOCKS_PER_SEC, end_t-start_t);

    return;

}

int main (int argc, char *argv []) {

    reduction();
    return 0;
}