#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/times.h>
#include <stdlib.h>

#define N 10000000
#define NUM_THREADS 20

void initialze_array(const int n, float *a, const float value);
void print_array(const int n, const float *a);

void initialze_array(const int n, float *a, const float value) {
    for (int i = 0; i < n; i++ ) {
      a[ i ] = value; 
   }
}

/*  
*  Reduction of 1/n series for 10 million numbers
*/
void reduction() {

    clock_t start, end;
    double start_t, end_t;
    float sum = 0.0;

    printf("\n\n==============PART 1: SEQUENTIAL ASCENDING=================\n");
    sum = 0.0;

    start = clock();
    start_t = omp_get_wtime(); 
    for(int i=1; i<=N; i++) {
        sum += 1.0/i;
    }
    end_t = omp_get_wtime(); 
    end = clock();
    printf(
    "SEQUENTIAL REDUCTION ASCENDING | float_ans: %0.20f, cpu time: %f, calculation time: %f\n", 
    sum, (end-start)/(double)CLOCKS_PER_SEC, end_t-start_t);

        printf("\n\n==============PART 2: SEQUENTIAL DESCENDING================\n");
    sum = 0.0;
    
    start = clock();
    start_t = omp_get_wtime(); 
    for (int i=N; i>=1; i--) {
        sum += 1.0/i;
    }
    end_t = omp_get_wtime(); 
    end = clock();

    printf(
    "SEQUENTIAL REDUCTION DESCENDING | float_ans: %0.20f, cpu time: %f, calculation time: %f\n", 
    sum, (end-start)/(double)CLOCKS_PER_SEC, end_t-start_t);

    printf("\n\n==============PART 3: OpenMP ASCENDING=================\n");

    omp_set_num_threads(NUM_THREADS);
    sum = 0.0;
    start = clock();
    start_t = omp_get_wtime(); 
    #pragma omp parallel for reduction(+:sum)
    for (int i=1; i<=N; i++) {
        sum += 1.0/i;
    }
    end_t = omp_get_wtime(); 
    end = clock();
    printf(
    "PARALLEL REDUCTION ASCENDING | float_ans: %0.20f, cpu time: %f, calculation time: %f\n", 
    sum, (end-start)/(double)CLOCKS_PER_SEC, end_t-start_t);

    printf("\n\n==============PART 4: OpenMP DESCENDING================\n");

    // although we run N to 1, we will not have control over threads acquire values of i
    // so we don't really know the actual sequence

    sum = 0.0;
    start = clock();
    start_t = omp_get_wtime(); 
    #pragma omp parallel for reduction(+:sum)
    for (int i=N; i>=1; i--) {
        sum += 1.0/i;
    }
    end_t = omp_get_wtime(); 
    end = clock();
    printf(
    "PARALLEL REDUCTION DESCENDING | float_ans: %0.20f, cpu time: %f, calculation time: %f\n", 
    sum, (end-start)/(double)CLOCKS_PER_SEC, end_t-start_t);

    return;

}

int main (int argc, char *argv []) {

    reduction();
    return 0;
}