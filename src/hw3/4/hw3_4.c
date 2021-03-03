#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {

    omp_set_num_threads(5);

    printf("\n\n=== SECTION 1 -- loop immediately after #pragma omp parallel\n");
    printf ("============================================================\n");
    #pragma omp parallel
    {
        for (int i=0; i<10; i++) {
            printf("for after par -- i: %d, thread num: %d, nthreads: %d\n", i, omp_get_thread_num(), omp_get_num_threads());
        }

        #pragma omp for
        for (int i = 0; i < 10; i++) {
            printf("omp for in par -- i: %d, thread num: %d, nthreads: %d\n", i, omp_get_thread_num(), omp_get_num_threads());
        }
    }

    printf ("\n\nSECTION 2 -- loop immediately after #pragma omp parallel for\n");
    printf ("============================================================\n");
    #pragma omp parallel for
    for (int i=0; i<10; i++) {
        printf("omp par for -- i: %d, thread num: %d, nthreads: %d\n", i, omp_get_thread_num(), omp_get_num_threads());
    }

    return 0;
}